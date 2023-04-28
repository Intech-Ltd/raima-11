/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2010 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

/************************** Globals *************************************/
var gUsername = '';
var gEnc = null;
var gTabObjs = new Array();
var gTabContainer;
var gButtons = new Array();
var gBoxes = new Array();
var gPagesInitialized = 0;
var gPageObjs = new Array();
var gDropdownClickState = false;
var gHelpClickState = false;
var gLoginRetry = 0;
var gBrowserIE = /MSIE (\d+\.\d+);/.test(navigator.userAgent);

var errors = new Array (
/* 0 */    "Invalid Request",
/* 1 */    "Invalid Login",
/* 2 */    "Session Timed Out",
/* 3 */    "Connection Send Error",
/* 4 */    "Connection Read Error",
/* 5 */    "Server Not Found",
/* 6 */    "Database is already replicated on slave server",
/* 7 */    "Database is already mirrored synchronously on master server",
/* 8 */    "Unable to connect to master server",
/* 9 */    "Unable to connect to slave server",
/* 10 */   "Slave is not replicating this database",
/* 11 */   "Connection must be made to a TFS"

);

var ERR_INVALIDREQUEST      = 0;
var ERR_LOGINFAILED         = 1;
var ERR_TIMEOUT             = 2;
var ERR_CONNECTIONSEND      = 3;
var ERR_CONNECTIONREAD      = 4;
var ERR_SERVERNOTFOUND      = 5;

var TAB_START_LEFT_POS      = 35;
var TAB_SPACER_WIDTH        = 5;
var TAB_BUTTON_WIDTH        = 28;
var TAB_PAGE_MARGIN         = 35;

var TABID_DOWNARROW         = -1;
var TABID_PLUS              = -2;

var BOUNDARY_BOX_INDENT     = 50;
var BOUNDARY_BOX_MARGIN     = 40;

var BOXSTATE_EXPANDED       = 1;
var BOXSTATE_COLLAPSED      = 2;

var PAGETYPE_TFS            = 0;
var PAGETYPE_DATABASE       = 1;
var PAGETYPE_LOGIN          = 2;
var PAGETYPE_CONNECT        = 3;

var PAGETYPE_UTILITY        = 4;

var MIRROR_PORT_OFFSET      = 1;

var NEW_TAB_TEXT            = 'New';

/* Help text */
var H_DATABASES = 'Click on a database name to open a new tab which will display specific database information.';
var H_USERS = 'All user connections to this server are displayed. Click the \'remove\' button to disconnect individual connections.  Make sure you really want to disconnect a user before doing so, as the user may have open database transactions.';
var H_DATAFLOW = 'If a mirroring or replication server is running, you will see any databases that are actively being mirrored or replicated.  You can begin mirroring or replication here.  First you must connect to at least one other server by opening a \'New\' tab and connecting.  To start mirroring or replication, you select a server from the \'Connected Masters\' dropdown, then select a database on that server from the \'Databases\' dropdown.  Once started, this server is a slave, receiving the mirrored/replicated database.<br><br>Options vary depending on the server type.  Mirroring servers will allow you to start synchronous mirroring.  SQL replication servers will allow you to select which SQL server will be receiving the data (RDMs, MySQL,...).  SQL replication also requires a DSN, unless you have set the environment variable RDSLOGIN prior to starting the SQL replication server.  For all servers, you can select \'Override In-Memory\' when the master database is in-memory but you want the slave database to be on disk. ';
var H_SETTINGS = 'The \'Create mirroring (or replication) log files\' boxes must be checked for the server to generate the necessary files for mirroring or replication to work properly.  Note that the database must be closed for the replication log file setting to take effect.  <br><br>Single slave mode is enabled by default.  To save space, this setting allows log files to remain on disk only until they are retrieved by the slave.  If a second slave is connected, this mode automatically switches off. <br><br>When multiple log files are retained on a server, the \'Keep log\' settings will tell the server when to clean up old log files.  Used space is determined per database.<br><br>These settings will be used by all databases on this server, unless they are overridden in the database\'s settings.';
var H_DBLOCKS = 'The status of the database file locks are displayed here.';
var H_DBUSERS = 'All users that have this database open are displayed here.';
var H_DBSETTINGS = 'These settings are for this specific database and override the settings used by this server.  Make your changes and click \'Apply\'.  To change this database to use the server\'s settings, click \'Default\'.';
var H_NEWCONNECTION = 'Enter the host name and port number of a server.  A connection will be made to this server and you will be able to modify this new server\'s settings.';
var H_SCHEMA = 'The schema information is displayed here.';

/* =======================================================================
    Return a message string given the idstring.
*/
function getErrorString(idstring)
{
    return errors[parseInt(idstring)];
}

/* =======================================================================
    Map an RDM error code in to a display.js error code.
*/
function mapRdmError(idstring)
{
    switch(idstring)
    {
        case '2':       /* S_NOTFOUND */
            return '10';
        case '3':       /* S_DUPLICATE */
            return '6';
        case '4':       /* PSP_NOSERVER */
            return '8';
        case '-88':     /* S_MIRSYNC_DENIED */
            return '7';
        case '-214':    /* S_TX_CONNECT */
            return '9';
    }
    return '0';
}

/* =======================================================================
    Convert the type response from the mirror/rep server to a display string
*/
function getMirServerType(server, sync, odbc)
{
    var type_str = '';

    if (sync == '1')
        type_str = 'Synchronous ';

    switch(server)
    {
        case 'RDM_DBMIRROR':
            type_str += 'Mirroring';
            break;
        case 'RDM_DBREP':
            type_str += 'Replicating';
            break;
        case 'RDM_DBREPSQL':
            type_str += 'SQL Replicating';
            break;
        default:
            return 'Invalid server type';
    }

    if (odbc != '')
        type_str += ' ' + odbc;

    return type_str;
}

/* =======================================================================
    Turn the given string into an AES encrypted digest, then convert
    that digest into a URL encoded string.
*/
function encryptAction(str)
{
    if (gEnc == null)
        return;

    var aes_digest = enc_encrypt(gEnc, str, str.length);
    var output = '';

    for (var ii = 0; ii < aes_digest.length; ++ii)
    {
        if (aes_digest[ii] < 16)
            output += '%0'+aes_digest[ii].toString(16);
        else
            output += '%'+aes_digest[ii].toString(16);
    }
    return output;
}

/* =======================================================================
    Request results from the server
    Use the given arguments as the 'action' and set the response 'handler'
    to receive the results of the request.
    filename is the filename portion of the URL.
    host and port are used with proxy.
    params means this request is a POST instead of a GET.
    userdata is attached to the xml request object and can be used by the handler
*/
function requestAction(actionstr, handler, filename, host, port, params, userdata)
{
    var enc_action = encryptAction(actionstr);
    var page;
    var reqtype = 'GET';
    var ajaxobj;

    page = '/'+filename+'?hostname='+host+'&port='+port+'&username='+gUsername+'&action='+enc_action;

    if (params)
        reqtype = 'POST';

    ajaxobj = callServer(reqtype, page, handler, params,
                         getSelectedTab(), userdata);
}

/* =======================================================================
    Take responseText and parse it into an array 'displayData', where
    each row contains an array of name/value data.

    Returns number of rows parsed.
*/
function parseResponse(responseText, displayData, first_item)
{
    var numrows = 0;
    var col = 0;
    var responseData = responseText.split("&");

    for (var ii = 0; ii < responseData.length; ++ii)
    {
        // Build array indexed by name returned from rdm
        var nvpair = responseData[ii].split("=");

        if (nvpair.length != 2)
            continue;

        if (nvpair[0] == first_item)
        {
            col = 0;
            ++numrows;
            displayData[numrows-1] = new Array();
        }

        // Unescape because the value might contain special chars
        nvpair[1] = unescape(nvpair[1]);

        displayData[numrows-1][col++] = nvpair[1];
    }

    return numrows;
}

/* =======================================================================
    Build html table rows for the given displayData and rdmNames.
    Outputs as many rows as data in displayData.
    Outputs as many columns as in rdmNames.

    Set rdmNames[xx] to '' to indicate which column will get the button.
    All rows get the same button.
    buttonFcns will take two arguments: buttonId and any argument passed in.

    buttonArgs is an integer column number of displayData (response).

    Returns HTML table rows.
*/
function buildDisplayRows(displayData, rdmNames, buttonName, buttonFcn, buttonArg)
{
    var html = "";
    var btnarg = new Array();

    for (var ii = 0; ii < displayData.length; ++ii)
    {
        if (displayData[ii][0] == "")
            continue;

        //Data row(s)
        html += '<tr>';
        for (var jj = 0; jj < rdmNames.length; ++jj)
        {
            if (rdmNames[jj] == '' && buttonName)
            {
                /* insert button container */
                html += '<td class="table-text-'+(ii%2+1)+'">';
                btnarg[0] = displayData[ii][buttonArg];
                html += generateButton(buttonName, buttonName+ii, null,
                                    buttonFcn, btnarg);
                html += '</td>';
            }
            else
                html += '<td class="table-text-'+(ii%2+1)+'">'+ displayData[ii][jj] +'</td>';
        }
        html += '</tr>';
    }
    return html;
}

/* =======================================================================
    Make sure response is valid.  Decrypt response text.
    Any server errors cause an alert.
    Returns decrypted response.
    'proxy' requests may timeout as the proxied server will not be hit
    all the time.  Ignore timeout error and allow for a retry.
*/
function validateLogin(responseText, proxy, connection)
{
    var unesc_resp_string;
    var unesc_resp_binary = new Array();
    var dec_response;
    var responseData;
    var errID;

    /* Zero length strings are valid when there are no results */
    if (!responseText || responseText.length == 0)
        return ' ';

    /* Check for errors on server - error response won't be encrypted */
    if (responseText.substr(0, 5) == 'Error')
    {
        errID = responseText.substr(6);
        if (parseInt(errID) == ERR_LOGINFAILED ||
                parseInt(errID) == ERR_INVALIDREQUEST)
            logoutClick();

        /* Ignore timeout errors from proxied servers. */
        if (proxy)
        {
            if (parseInt(errID) == ERR_TIMEOUT)
                return '';
        }
        else
            closeDeadTabs(connection);

        if (connection.errorDisplaying == 0)
        {
            connection.errorDisplaying = 1;
            alert("Error: "+getErrorString(errID));
            connection.errorDisplaying = 0;
        }
        return '';
    }
    else if (responseText.substr(0, 7) == 'Warning')
    {
        /* For proxy requests, handle server not found warning */
        if (proxy && parseInt(responseText.substr(8)) == ERR_SERVERNOTFOUND)
        {
            if (connection != undefined && connection.identity != '')
            {
                connection.identity = '';
                displayDataflowIdentity();
            }
        }
        else
        {
            if (connection.errorDisplaying == 0)
            {
                connection.errorDisplaying = 1;
                alert('Warning: '+getErrorString(responseText.substr(8)));
                connection.errorDisplaying = 0;
            }
            if (getSelectedTab().getText() != NEW_TAB_TEXT)
                closeDeadTabs(connection);
        }
        return '';
    }

    unesc_resp_string = unescape(responseText);

    /* response is a string, convert to binary array */
    for (var ii = 0; ii < unesc_resp_string.length; ++ii)
        unesc_resp_binary[ii] = unesc_resp_string.charCodeAt(ii);

    dec_response = enc_decrypt(gEnc, unesc_resp_binary, unesc_resp_binary.length);

    /* All responses from the server will end with '&' and have an '='.
       If no '&' or '=', it means we could not decrypt the results successfully.
     */
    responseData = dec_response.split("&");
    if (responseData.length >= 1)
    {
        // Build array indexed by name returned from rdm
        var nvpair = responseData[0].split("=");

        if (nvpair.length == 0)
        {
            /* error decrypting */
            logoutClick();
            return '';
        }
    }
    else
    {
        /* error decrypting */
        logoutClick();
        return '';
    }

    return dec_response;
}

/* =======================================================================
    If the user connected to the localhost, get the hostname.  This is
    for mirroring, so the slave can connect to the correct host if the
    slave is on a different server.  The 'gethostname' returns the name
    in an ajax handler, so use the localhost here and let the callback
    replace the tab's text.
*/
function updateLocalhostName(connection)
{
    var actionstr = 'action=gethostname&';

    if (connection.host == '127.0.0.1' || connection.host == 'localhost')
        requestAction(actionstr, ajaxGetHostName, 'proxy.rdm',
                        connection.host, parseInt(connection.port), null, null);
}

/* =======================================================================
    Any click should close the dropdown.
*/
function bodyClick()
{
    var container = document.getElementById('dropdownContainer');

    if (gDropdownClickState == false && container.style.visibility == 'visible')
        container.style.visibility = 'hidden';

    container = document.getElementById('helpContainer');
    if (gHelpClickState == false && container.style.visibility == 'visible')
        container.style.visibility = 'hidden';

    gDropdownClickState = false;
    gHelpClickState = false;
}

/* =======================================================================
    Change the selected tag and change the display.
*/
function tabClick(tabid)
{
    tabSelect(tabid);
    resumeRefresh();

    reloadPage();

    return false;
}

/* =======================================================================
    Close the selected tab
*/
function tabCloseClick(buttonID, tabid)
{
    var end = gTabObjs.length;
    var tabIndex = parseInt(tabid);

    if (tabIndex >= end || tabIndex < 0)
        return;

    // IE doesn't call bodyClick on this
    bodyClick();

    removeTab(buttonID, tabid);

    resumeRefresh();

    reloadPage();
    return false;
}


/* =======================================================================
    Open a new tab for this database
*/
function dbNameClick(id)
{
    /* Database pages are opened to the same connection as the tfs page */
    var tab = getSelectedTab();
    var tabname = tab.connection.dbobjs[id].dbname+'@'+tab.connection.host+':'+tab.connection.port
    var tabobj;

    if (duplicateTabCheck(tabname) == false)
    {
        tabobj = addTab(PAGETYPE_DATABASE, tabname, tab.connection);
        tabobj.setDbID(id);
    }

    setCookie('tabpage-databases', id);

    setPageType(PAGETYPE_DATABASE);

    window.scrollTo(0,0);

    reloadPage();

}

/* =======================================================================
    Select the given tabid
*/
function dropdownClick(tabid)
{
    tabClick(tabid);
}

/* =======================================================================
    Show dropdown with all tabs listed for easy navigation.
*/
function downArrowClick()
{
    var container = document.getElementById('dropdownContainer');
    var hiddenContainer = document.getElementById('hiddenContainer');
    var maxwidth = 0;
    var ii;
    var html = '<table border="0">';
    var tabtext;

    if (container.style.visibility == 'visible')
        container.style.visibility = 'hidden';
    else
    {
        /* <body> has an onClick, so we don't want it closing the dropdown
           on the open click.
         */
        gDropdownClickState = true;
        for (ii = 0; ii < gTabObjs.length; ++ii)
        {
            tabtext = gTabObjs[ii].getText();

            /* to dynamically size the dropdown, calculate maxwidth */
            hiddenContainer.innerHTML = '<span class="dropdown-text-off">'+tabtext+'</span>';
            maxwidth = Math.max(maxwidth, hiddenContainer.clientWidth + 10);

            html += '<tr><td id="dropdownEntry'+ii+'" class="dropdown-text-off" onClick="dropdownClick('+ii+')" onMouseOver="document.getElementById(\'dropdownEntry'+ii+'\').className=\'dropdown-text-on\';document.body.style.cursor=\'pointer\'" onMouseOut="document.getElementById(\'dropdownEntry'+ii+'\').className=\'dropdown-text-off\';document.body.style.cursor=\'auto\'">' + tabtext + '</td></tr>';
        }
        container.innerHTML = html + "</table>";
        container.style.width = maxwidth + "px";
        container.style.left = (gTabContainer.leftpos + gTabContainer.width - maxwidth) + "px";
        container.style.visibility = 'visible';
    }
    return false;
}

/* =======================================================================
    Add a new tab
*/
function plusClick()
{
    addTab(PAGETYPE_CONNECT, NEW_TAB_TEXT, null);
    setPageType(PAGETYPE_CONNECT);

    // IE doesn't call bodyClick on this
    bodyClick();

    reloadPage();
    return false;
}

/* =======================================================================
*/
function applySettingsClick(buttonID, arg)
{
    var actionstr;
    var agevalue;
    var inivalue;
    var spacevalue;
    var selectedUnit;
    var actionPrefix = 'action=setini&';
    var idPrefix = '';
    var tab = getSelectedTab();

    if (arg != '')
    {
        actionPrefix += 'dbname=' + arg + '&';
        idPrefix = 'db';
    }

    agevalue = document.getElementById(idPrefix+'FlowLogAge').value;
    agevalue.replace(/^\s+/g, "").replace(/\s+$/g, "");
    spacevalue = document.getElementById(idPrefix+'FlowLogSpace').value;
    spacevalue.replace(/^\s+/g, "").replace(/\s+$/g, "");
    if (isNaN(agevalue-0) || isNaN(spacevalue-0) || parseInt(agevalue) < 0 ||
            parseInt(spacevalue) < 0)
    {
        alert("Invalid input: Age and space fields must be numbers greater than 0");
        return;
    }

    gButtons[buttonID].disableButton();

    actionstr = actionPrefix + 'section=configuration&key=mirroring&';
    inivalue = document.getElementById(idPrefix+'SettingsMirLog').checked == true ? '1' : '0';
    actionstr += 'value='+inivalue+'&';
    requestAction(actionstr, ajaxINISetHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, null);

    actionstr = actionPrefix + 'section=configuration&key=replication&';
    inivalue = document.getElementById(idPrefix+'SettingsRepLog').checked == true ? '1' : '0';
    actionstr += 'value='+inivalue+'&';
    requestAction(actionstr, ajaxINISetHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, null);

    actionstr = actionPrefix + 'section=configuration&key=SingleSlaveMode&';
    inivalue = document.getElementById(idPrefix+'SettingsSingleSlave').checked == true ? '1' : '0';
    actionstr += 'value='+inivalue+'&';
    requestAction(actionstr, ajaxINISetHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, null);

    actionstr = actionPrefix + 'section=LogCleanup&key=LogFileAge&';
    selectedUnit = document.getElementById(idPrefix+'SettingsMirLogAge');
    if (agevalue != '')
        agevalue += selectedUnit.options[selectedUnit.selectedIndex].value;
    actionstr += 'value='+agevalue+'&';
    requestAction(actionstr, ajaxINISetHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, null);

    actionstr = actionPrefix + 'section=LogCleanup&key=LogFileSpace&';
    selectedUnit = document.getElementById(idPrefix+'SettingsMirLogSpace');
    if (spacevalue != '')
        spacevalue += selectedUnit.options[selectedUnit.selectedIndex].value;
    actionstr += 'value='+spacevalue+'&';
    requestAction(actionstr, ajaxINISetHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, buttonID);

}

/* =======================================================================
    Revert the INI to the tfs' settings by removing the dbname.ini file.
*/
function revertSettingsClick(buttonID, arg)
{
    var actionstr = 'action=revertini&dbname='+arg+'&';
    var tab = getSelectedTab();

    gButtons[buttonID].disableButton();

    requestAction(actionstr, ajaxINIRevertHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, buttonID);
}


/* =======================================================================
    Stop the slave mirroring/replicating.
*/
function stopSlaveClick(buttonID, slavedb, slavehost, slaveport)
{
    var actionstr;
    var tab = getSelectedTab();

    /* Normalize dbname */
    slavedb = slavedb.replace(/\//g, "\\");
    actionstr = "action=stopflow&slavehost="+slavehost+"&slaveport="+slaveport+"&dbname="+slavedb;

    pauseRefresh();
    gButtons[buttonID].disableButton();

    /* Send request to connection's host:port, stop slave on slavehost:slaveport */
    requestAction(actionstr, ajaxFlowStopHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port) + MIRROR_PORT_OFFSET, null, buttonID);
}

/* =======================================================================
    Start the slave mirroring/replicating
*/
function startSlaveClick(buttonID, arg)
{
    var tab = getSelectedTab();
    var actionstr;
    var selectedMaster = document.getElementById('StartFlowSelectMaster');
    var selectedDb = document.getElementById('StartFlowSelectDbs');
    var mastertab = gTabObjs[selectedMaster.options[selectedMaster.selectedIndex].value];
    var masterName = mastertab.getText();
    var override = document.getElementById('StartFlowOverrideInMem').checked;
    var synch = false;
    var sql = '';
    var dsn = '';
    var sqlobj;
    var ii;
    var dbobj;

    dbobj = getDbObjByName(mastertab.connection,
                           selectedDb.options[selectedDb.selectedIndex].value);

    if ((tab.connection.identity == 'RDM_DBMIRROR' &&
                                dbobj.mirroring != 1) ||
        ((tab.connection.identity == 'RDM_DBREP' ||
            tab.connection.identity == 'RDM_DBREPSQL') &&
                                dbobj.replication != 1))
    {
        alert('Database \''+selectedDb.options[selectedDb.selectedIndex].value+
                '\' on master server ('+mastertab.connection.host+':'+
                mastertab.connection.port+') not configured to generate log files');
        return;
    }

    if (tab.connection.identity == 'RDM_DBREPSQL')
    {
        sqlobj = document.getElementById('StartFlowSQLGroup');
        for (ii = 0; ii < sqlobj.length; ++ii)
        {
            if (sqlobj[ii].checked)
            {
                sql = sqlobj[ii].value;
                break;
            }
        }
        dsn = document.getElementById('StartFlowSQLDSN').value;
    }

    if (tab.connection.identity == 'RDM_DBMIRROR')
        synch = document.getElementById('StartFlowSynchronous').checked;

    actionstr = 'action=startflow&slavehost=' + tab.connection.host +
                    '&slaveport=' + (tab.connection.port) +
                    '&dbname=' + selectedDb.options[selectedDb.selectedIndex].value +
                    '@' + masterName + '&';
    if (override)
        actionstr += 'override_inmem=1&';
    if (synch)
        actionstr += 'sync=1&';
    else
        actionstr += 'unsync=1&';
    if (sql != '')
        actionstr += 'format='+sql+'&';
    if (dsn != '')
        actionstr += 'dsn='+dsn+'&';

    gButtons[buttonID].disableButton();

    requestAction(actionstr, ajaxFlowStartHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port) + MIRROR_PORT_OFFSET, null, buttonID);
}


/* =======================================================================
*/
function loginClick(buttonID, arg)
{
    var digest;
    var password = document.getElementById('password').value;

    digest = sha_encrypt(password, password.length);

    /* Keep enc context global for this browser, since only one person can
       be logged in inside this browser.
     */
    gEnc = enc_init(digest);
    gUsername = document.getElementById('username').value;
    document.body.style.cursor='auto';

    /* Remove key handler so repeated 'enter' can't be hit */
    document.getElementById('password').onkeydown = "";

    setPageType(PAGETYPE_TFS);

    updateLocalhostName(gTabObjs[0].connection);

    getIdentity(); // Get mirroring server's identity (mir/rep/repsql)

    /* Reget db names in case new db added after logging out */
    getDbNames(); // This will cause two login error messages if pswd invalid.

    resumeRefresh();
}

/* =======================================================================
*/
function logoutClick()
{
    var ii;
    var end = gTabObjs.length;

    gEnc = null;
    gUsername = '';

    setPageType(PAGETYPE_LOGIN);
    setCookie('tabpage-system', 0);
    setCookie('tabpage-system-lefttab', 0);

    for (ii = 1; ii < end; ++ii)
        removeTab('', 1);

    tabSelect(0);

    reloadPage();

    document.getElementById('logoutDiv').style.visibility = 'hidden';
}

/* =======================================================================
    Connect to another TFS.
*/
function connectClick(buttonID)
{
    var host = document.getElementById('connect_hostname').value;
    var port = document.getElementById('connect_port').value;
    var tab = getSelectedTab();
    var oldtab;
    var actionstr = "action=dblist";
    var connection;
    var ii;

    if (host == '' || port == '' || isNaN(port-0))
    {
        alert("Must enter a host and port");
        setTimeout(function() { document.getElementById('connect_hostname').focus(); }, 200);
        return;
    }

    /* Don't duplicate tabs */
    if (duplicateTabCheck(host + ':' + port) == true)
    {
        oldtab = getSelectedTab();
        if (oldtab.tabid != tab.tabid)
        {
            tabCloseClick('CloseTab'+tab.tabid, tab.tabid);
            return;
        }
    }

    connection = new connectionObj(host, port);
    setCurrentTabConnection(connection);
    requestAction(actionstr, ajaxConnectResponseHandler, 'proxy.rdm', host, port, null, buttonID);

    gButtons[buttonID].disableButton();
}

/* =======================================================================
    Disconnect this user from the TFS.
*/
function disconnectUserClick(buttonID, username)
{
    var actionstr = 'action=disconnectUser&disuser='+username+'&';
    var tab = getSelectedTab();

    gButtons[buttonID].disableButton();

    requestAction(actionstr, ajaxUserDisconnect, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, buttonID);
}

/* =======================================================================
*/
function helpClick(buttonID, helpid)
{
    var left = 0;
    var top = 0;
    var obj;
    var btn = document.getElementById(buttonID);
    var container = document.getElementById('helpContainer');
    var event = window.event;

    if (event)
    {
        /* Opera doesn't work with offsetTop */
        left = event.pageX;
        top = event.pageY;

        if (left == undefined)
        {   // IE has event, but not pageX|Y
            left = 0;
            top = 0;
        }
    }

    if (left == 0)
    {
        obj = btn;
        do
        {
            left += obj.offsetLeft;
            top += obj.offsetTop;
        } while (obj = obj.offsetParent);
    }

    if (container.style.visibility == 'visible' &&
            parseInt(container.style.top) == top + 5)
        container.style.visibility = 'hidden';
    else
    {
        container.style.visibility = 'visible';
        gHelpClickState = true;

        container.style.left = (left - container.offsetWidth) + "px";
        container.style.top = (top + 5) + "px";
        container.innerHTML = getButtonHelpText(buttonID);
    }
}

/* =======================================================================
    pageObj method: make all parts of page visible
*/
function makePageVisible()
{
    var tab_container = document.getElementById('tabContainer');
    var container = document.getElementById(this.id);

    if (this.tabs)
        tab_container.style.visibility = 'visible';
    else
        tab_container.style.visibility = 'hidden';

    this.visible = true;
    container.style.visibility = 'visible';
    container.style.height = 'auto';
    container.style.display = 'inline';
}

/* =======================================================================
    pageObj method: make all parts of page hidden
*/
function makePageHidden()
{
    var container = document.getElementById(this.id);

    this.visible = false;
    container.style.visibility = 'hidden';
    container.style.height = "0px";
    container.style.display = 'none';
}

/* =======================================================================
    pageObj method: Insert a new box after the given 'offset' (0 relative).
    'offset' is the vertical position on the page, 0 being the topmost box.
    'refreshfcn' is called during refresh to fill in the box.
    Generates boxObj and stores in 'boxes' array.
*/
function insertPageBox(offset, id, title, state, children, helptext, refreshfcn)
{
    var parentBoxId;
    var container;
    var newBoxDiv = document.createElement("div");
    var oldBoxDiv;
    var box;
    var emptyDiv;
    var numBoxes = this.boxes.length;
    var ii;

    newBoxDiv.setAttribute("id", id);

    switch (this.type)
    {
        case PAGETYPE_TFS:
            parentBoxId = 'TFSPageContainer';
            emptyDiv = 'TFSEMPTYDIV';
            break;
        case PAGETYPE_DATABASE:
            parentBoxId = 'DbPageContainer';
            emptyDiv = 'DBEMPTYDIV';
            break;
        default:
            alert('Unsupported page type');
            break;
    }

    container = document.getElementById(parentBoxId);

    if (numBoxes == 0)
    {
        oldBoxDiv = document.getElementById(emptyDiv);
        oldBoxDiv = oldBoxDiv.nextSibling;
    }
    else if (offset < numBoxes)
        oldBoxDiv = document.getElementById(this.boxes[offset].id);
    else // offset >= length - append at end
    {
        oldBoxDiv = document.getElementById(this.boxes[numBoxes-1].id);
        oldBoxDiv = oldBoxDiv.nextSibling;
    }

    container.insertBefore(newBoxDiv, oldBoxDiv);

    document.getElementById(id).className = 'boxContainer';

    box = generateBox(id, title, state, children, helptext, refreshfcn);

    this.boxes.splice(offset, 0, box);

    /* reset the borders in case a box was inserted in the middle or at the end
     */
    for (ii = 0; ii < this.boxes.length - 1; ++ii)
    {
        document.getElementById(this.boxes[ii].id).style.borderStyle = "solid solid none solid";
    }
    // bottom box must have a solid bottom border
    document.getElementById(this.boxes[this.boxes.length-1].id).style.borderStyle = "solid";
}

/* =======================================================================
    pageObj method: call refresh for each box on a page.
*/
function refreshPageObj()
{
    var ii;
    for (ii = 0; ii < this.boxes.length; ++ii)
    {
        this.boxes[ii].refresh();
    }
}

/* =======================================================================
    pageObj controls what boxes are displayed based on the page type.
    'type' is PAGETYPE_*
    'id' is string name id of this page object.
    'visible' is boolean
    'tabs' is boolean if tabs are displayed on this page
*/
function pageObj(type, id, visible, tabs)
{
    this.id = id;
    this.type = type;
    this.tabs = tabs;
    this.makeVisible = makePageVisible;
    this.makeHidden = makePageHidden;
    this.insertBox = insertPageBox;
    this.refresh = refreshPageObj;
    this.visible = visible;
    this.boxes = new Array();
}

/* =======================================================================
    Switch pages - show boxes of given page type and hide all others.
*/
function setPageType(pagetype)
{
    for (var ii = 0; ii < gPageObjs.length; ++ii)
    {
        if (pagetype == gPageObjs[ii].type)
            gPageObjs[ii].makeVisible();
        else
            gPageObjs[ii].makeHidden();
    }
}

/* =======================================================================
    Returns type of current page
*/
function getPageType()
{
    for (var ii = 0; ii < gPageObjs.length; ++ii)
    {
        if (gPageObjs[ii].visible)
            return gPageObjs[ii].type;
    }
    return PAGETYPE_LOGIN;
}

/* =======================================================================
*/
function initServerPage()
{
    var connection;
    var hostname = window.location.hostname;
    var port = window.location.port;

    connection = new connectionObj(hostname, port);

    generateUtilityTabs();
    gTabContainer = new tabContainerObj();

    addTab(PAGETYPE_TFS, hostname+':'+port, connection);
}

/* =======================================================================
    Create page objects.
    There are four types of pages. Pages that show information about a TFS
    and pages that show information about a database, a page that prompts
    for a login and a page that prompt for connection info.

    The Dataflow container holds three children containers, master, slave and
    start.
*/
function initPages()
{
    var tfs_div;
    var db_div;
    var sys_div;
    var dfChildren = new Array('MasterFlowContainer', 'SlaveFlowContainer', 'StartFlowContainer');
    /* Settings box has one child.  Child level needed because it contains
       controls (text, checkbox, dropdowns), otherwise display is broken.
     */
    var settingsChildren = new Array('DataflowSettings');
    var dbSettingsChildren = new Array('dbDataflowSettings');

    gPagesInitialized = 1;

    /* divs TFSEMPTYDIV and DBEMPTYDIV are empty.  They keep dropdowns from
       closing during refresh on Firefox.  The are also used as place holders
       for pageObj.insertBox.
     */
    tfs_div = '<div id="TFSPageContainer"><div id="TFSEMPTYDIV"></div></div>';
    db_div = '<div id="DbPageContainer"><div id="DBEMPTYDIV"></div></div>';
    sys_div = '<div id="ConnectContainer"></div><div id="LoginContainer"></div>';

    document.getElementById('pageContainer').innerHTML = '<br>' + tfs_div + db_div + sys_div + '<br><br>';

    /* A TFS info page contains database, user and dataflow info */
    gPageObjs[PAGETYPE_TFS] = new pageObj(PAGETYPE_TFS, 'TFSPageContainer', false, true);
    initServerPage();

    gPageObjs[PAGETYPE_TFS].insertBox(0, 'DatabaseContainer', 'Databases',
                    BOXSTATE_EXPANDED, null, H_DATABASES, null);
    gPageObjs[PAGETYPE_TFS].insertBox(1, 'UsersContainer', 'Users',
                    BOXSTATE_COLLAPSED, null, H_USERS, refreshUsersContainer);
    gPageObjs[PAGETYPE_TFS].insertBox(2, 'DataflowContainer', 'Dataflow',
                    BOXSTATE_COLLAPSED, dfChildren, H_DATAFLOW,
                    refreshDataflowContainer);
    gPageObjs[PAGETYPE_TFS].insertBox(3, 'DataflowSettingsContainer',
                    'Dataflow Settings', BOXSTATE_COLLAPSED, settingsChildren, H_SETTINGS, null);

    /* A database info page contains locks, user and file info */
    gPageObjs[PAGETYPE_DATABASE] = new pageObj(PAGETYPE_DATABASE, 'DbPageContainer', false, true);

    gPageObjs[PAGETYPE_DATABASE].insertBox(0, 'DbLocksContainer', 'Locks',
                    BOXSTATE_EXPANDED, null, H_DBLOCKS, refreshDbLocksContainer);
    gPageObjs[PAGETYPE_DATABASE].insertBox(1, 'DbUsersContainer',
                    'Users with database open', BOXSTATE_COLLAPSED, null,
                    H_DBUSERS, refreshDbUsersContainer);
    gPageObjs[PAGETYPE_DATABASE].insertBox(2, 'SchemaContainer', 'Schema',
                    BOXSTATE_COLLAPSED, null, H_SCHEMA, refreshSchemaContainer);
    gPageObjs[PAGETYPE_DATABASE].insertBox(3, 'DbDataflowSettingsContainer',
                    'Dataflow Settings', BOXSTATE_COLLAPSED, dbSettingsChildren,
                    H_DBSETTINGS, null);

    /* Connect and Login pages have no boxes */
    gPageObjs[PAGETYPE_CONNECT] = new pageObj(PAGETYPE_CONNECT, 'ConnectContainer', false, true)
    gPageObjs[PAGETYPE_LOGIN] = new pageObj(PAGETYPE_LOGIN, 'LoginContainer', true, false);
}

/* =======================================================================
    databaseObj keeps info about each database on a specific server.
    'mirroring' and 'replication' are the server side settings to generate
    log files.  We need a local copy here so we can prevent the user from
    starting mirroring when the server isn't setup for it.  Due to the
    asynchronous nature of Ajax, we can't get the ini settings when we need
    them because the response might come after when we need them.
*/
function databaseObj(dbname, mirroring, replication)
{
    this.dbname = dbname;
    this.mirroring = mirroring;
    this.replication = replication;
    this.records = new Array(); // array of recordObjs
    this.fields = new Array();  // array of fieldObjs
    this.sets = new Array();    // array of setObjs
    this.files = new Array();   // array of fileObjs
}

/* =======================================================================
    Returns the dbobj for a given dbname in the given connection.
    Returns null if dbname not found.
*/
function getDbObjByName(connection, dbname)
{
    var ii;

    for (ii = 0; ii < connection.dbobjs.length; ++ii)
    {
        if (connection.dbobjs[ii].dbname == dbname)
            return connection.dbobjs[ii];
    }
    return null;
}


/* =======================================================================
    connectionObj keeps TFS server connection info.
*/
function connectionObj(host, port)
{
    this.host = host;
    this.port = port;
    this.identity = '';
    /* Use this to help limit the number of alerts displayed on an error.
       There are many requests to a server and if one fails they will likely
       all fail. The errors will come back at different times, so this won't
       get them all.
     */
    this.errorDisplaying = 0;

    // One databaseObj per database on this connection
    this.dbobjs = new Array();
}


/* =======================================================================
    Change the display of which tab is selected.
*/
function tabSelect(tabid)
{
    var cookiename = 'tabpage-system';
    var tab;
    var tabtext;
    var original_tab = getCookie(cookiename);
    var old_tabid = parseInt(original_tab);

    if (original_tab == undefined || original_tab == '')
        old_tabid = 0;
    if (tabid == undefined || tabid == '')
        tabid = '0';

    tab = document.getElementById('pageTab'+old_tabid);
    tabtext = document.getElementById("pageTabText"+old_tabid);
    if (tab)
    {
        tab.style.zIndex = -1;
        tab.className = 'pagetab-text-off';
        tabtext.className = 'pagetab-text-off';
    }

    tabid = parseInt(tabid);
    tab = document.getElementById('pageTab'+tabid);
    tabtext = document.getElementById('pageTabText'+tabid);
    tab.style.zIndex = 2;
    tab.className = 'pagetab-text-on';
    tabtext.className = 'pagetab-text-on';

    setCookie(cookiename, tabid);

    if (gTabObjs[tabid].tabtype == PAGETYPE_DATABASE)
        setCookie('tabpage-databases', gTabObjs[tabid].dbid);

    gTabContainer.scrollToTab(tabid);

    gTabContainer.showTabs();
}

/* =======================================================================
    tabObj Method: reset tab's width
    Uses tab text style 'pagetab-text-on' to determine width.
*/
function setTabWidth()
{
    var padding = 0;
    var hiddenContainer = document.getElementById('hiddenContainer');
    var dots_width;

    // to dynamically size the tabs
    hiddenContainer.innerHTML = '<span class="pagetab-text-on">...</span>';
    dots_width = hiddenContainer.clientWidth;

    hiddenContainer.innerHTML = '<span class="pagetab-text-on">'+this.tabtext+'</span>';

    /* Add padding for the close 'x' button. */
    if (this.tabtype != PAGETYPE_UTILITY)
    {
        padding = 25;

        if (hiddenContainer.clientWidth + 10 + padding > gTabContainer.width - TAB_PAGE_MARGIN)
        {
            padding += dots_width;

            while (hiddenContainer.clientWidth + 10 + padding > gTabContainer.width - TAB_PAGE_MARGIN - (2*TAB_BUTTON_WIDTH))
            {
                this.tabtext = this.tabtext.substring(0, this.tabtext.length-2);
                hiddenContainer.innerHTML = '<span class="pagetab-text-on">'+this.tabtext+'...</span>';
            }
            this.tabtext += '...';
        }
    }

    this.width = hiddenContainer.clientWidth + 10 + padding;
}

/* =======================================================================
    tabObj method: set tab's text
*/
function setTabText(newText)
{
    this.tabtext = newText;
}

/* =======================================================================
    tabObj method: get tab's text
*/
function getTabText()
{
    return this.tabtext;
}

/* =======================================================================
    tabObj method: set tab's dbid
*/
function setTabDbID(newID)
{
    if (this.tabtype == PAGETYPE_DATABASE)
        this.dbid = newID;
}

/* =======================================================================
    tabObj method: Fill in the tab div with html link tag with a click action.
*/
function generateTabContent()
{
    var tab;
    var tabclass;
    var leftpos;
    var buttonHTML = '&nbsp;';
    var tabargs = new Array();

    tab = document.getElementById('pageTab'+this.tabid);

    tabclass = 'pagetab-text-off';
    tab.style.zIndex = 0;
    if (this.tabid == 0)
        gTabObjs[0].leftpos = TAB_START_LEFT_POS;

    tabargs[0] = this.tabid;
    if (this.tabid > 0)
        buttonHTML = generateButton('Close', 'CloseTab'+this.tabid, 'x12r.png', 'tabCloseClick', tabargs);

    tab.innerHTML='<table border="0"><col width="'+(this.width-12)+'"><col width="12"><tr><td><a id="pageTabText'+this.tabid+'" onClick="tabClick('+this.tabid+');return false;" href="javascript:void(0);" class="'+tabclass+'">&nbsp;'+this.getText()+'</a></td><td>'+buttonHTML+'</td></tr></table>';
}


/* =======================================================================
    Tab object constructor
    <div> tag must be created before creating this object.
*/
function tabObj(tabtype, tabid, tabtext, connection)
{
    var container;
    var divname;

    this.tabid = tabid;
    this.tabtext = tabtext;
    this.tabtype = tabtype;
    this.connection = connection;
    this.dbid = -1;

    this.setWidth = setTabWidth;
    this.setText = setTabText;
    this.getText = getTabText;
    this.setDbID = setTabDbID;
    this.generate = generateTabContent;

    this.setWidth();

    switch (tabid)
    {
        case TABID_DOWNARROW:
            divname = 'downArrow';
            this.width = TAB_BUTTON_WIDTH;
            break;
        case TABID_PLUS:
            divname = 'plusTab';
            this.width = TAB_BUTTON_WIDTH;
            break;
        default:
            divname = 'pageTab'+tabid;
            break;
    }

    container = document.getElementById(divname);
    container.style.visibility = 'hidden';
    container.style.position = 'absolute';
    container.style.top = '8px';
    container.style.height = '28px';
    container.style.textAlign = 'center';
    container.style.borderStyle = 'solid solid hidden solid';
    container.style.borderWidth = '1px';
    container.style.borderColor = '#000000';
    container.style.lineHeight = '1.5em';
    container.style.borderRadius = '3px 3px 0px 0px';
    container.style.width = this.width;

    this.leftpos = 0;
}

/* =======================================================================
*/
function getSelectedTab()
{
    var tabid = getCookie('tabpage-system');

    if (tabid)
        return gTabObjs[tabid];
    return null;
}

/* =======================================================================
*/
function getCurrentTabType()
{
    var tabid = getCookie('tabpage-system');

    if (tabid)
        return gTabObjs[tabid].tabtype;
    else
        return PAGETYPE_LOGIN;
}

/* =======================================================================
*/
function setCurrentTabType(newtype)
{
    var tabid = getCookie('tabpage-system');

    if (tabid)
        gTabObjs[tabid].tabtype = newtype;
}

/* =======================================================================
*/
function setCurrentTabConnection(connection)
{
    var tabid = getCookie('tabpage-system');

    if (tabid)
        gTabObjs[tabid].connection = connection;
}


/* =======================================================================
    Add the dropdown arrow tab.
*/
function addArrowTab(arrowLeft)
{
    var tab;
    var leftmost = getCookie('tagpage-system-lefttab');

    tab = document.getElementById('downArrow');
    tab.style.zIndex = 0;
    gTabObjs['down'].leftpos = arrowLeft;
    tab.style.left = arrowLeft + "px";
    tab.style.visibility = 'inherit';
    if (leftmost == 0)
        tabclass = 'arrowtab-text-off';
    else
        tabclass = 'arrowtab-text-on';
    tab.className = tabclass;
    tab.innerHTML='<table border="0"><tr><td>'+generateButton('All Tabs', 'DownTabText', 'down_arrow.png', 'downArrowClick', null)+'</td></tr></table>';
    tab.style.width = TAB_BUTTON_WIDTH + "px"; //TODO: calculate actual button size
}

/* =======================================================================
    Add the plus tab.
*/
function setPlusTab(leftpos)
{
    var tab;

    tab = document.getElementById('plusTab');
    tab.style.zIndex = 0;
    gTabObjs['plus'].leftpos = leftpos;
    tab.style.left = leftpos + "px";
    tab.style.visibility = 'inherit';
    tab.className = 'pagetab-text-off';

    tab.innerHTML='<center><table border="0"><tr><td>'+generateButton('New Tab', 'PlusTabText', 'plus12.png', 'plusClick', null)+'</td></tr></table></center>';
    tab.style.width = TAB_BUTTON_WIDTH + "px"; //TODO: calculate actual button size
}

/* =======================================================================
    Create utility tabs (arrows, plus).
    Inserts created tab objects into gTabObjs.
*/
function generateUtilityTabs()
{
    var ii;
    var tabText;

    tabText = '<div id="downArrow"></div><div id="plusTab"></div><div id="dropdownContainer"></div>';

    // Rewrite tabContainer with given number of pageTabs
    document.getElementById('tabContainer').innerHTML = tabText;


    gTabObjs['down'] = new tabObj(PAGETYPE_UTILITY, TABID_DOWNARROW, 'v', undefined);

    gTabObjs['plus'] = new tabObj(PAGETYPE_UTILITY, TABID_PLUS, '+', undefined);
}


/* =======================================================================
    Add a new tab page on the right.
*/
function addTab(tabType, tabName, connection)
{
    var plustab = gTabObjs['plus'];
    var end = gTabObjs.length;
    var tab;

    gTabContainer.insert('pageTab'+end);

    gTabObjs[end] = new tabObj(tabType, end, tabName, connection);

    gTabObjs[end].generate();

    tabSelect(end);

    return gTabObjs[end];
}

/* =======================================================================
    Rename an existing tab, resize and adjust position of all tabs to the right
*/
function renameTab(tabid, newName)
{
    var tab = document.getElementById('pageTab'+tabid);
    var end = gTabObjs.length;
    var tabIndex = parseInt(tabid);

    gTabObjs[tabIndex].setText(newName);
    gTabObjs[tabIndex].setWidth();
    gTabObjs[tabIndex].generate();
    tab.style.width = gTabObjs[tabIndex].width + "px";

    /* move all tabs right by the difference in the size of the renamed tab */
    for (var ii = tabIndex+1; ii < end; ++ii)
    {
        gTabObjs[ii].leftpos = gTabObjs[ii-1].leftpos + gTabObjs[ii-1].width + TAB_SPACER_WIDTH;
    }

    tabSelect(tabIndex);
}

/* =======================================================================
    Remove an existing tab and adjust position of all tabs to the right.
*/
function removeTab(buttonID, tabid)
{
    var tab;
    var tabIndex = parseInt(tabid);
    var end = gTabObjs.length;
    var original_tab = parseInt(getCookie('tabpage-system'));

    gTabContainer.remove('pageTab'+tabid);

    /* Consolidate gTabObjs, removing the obj at tabid */
    gTabObjs.splice(parseInt(tabid), 1);

    /* adjust position of all tabs to the right */
    /* move all tabs left by the size of the removed tab */
    for (var ii = tabIndex+1; ii < end; ++ii)
    {
        /* need to subtract one from all div ids */
        tab = document.getElementById('pageTab'+ii);
        tab.id = 'pageTab'+(ii-1);
        gTabObjs[ii-1].tabid--;

        /* Must regenerate content of tab as it has old index in tabClick call*/
        gTabObjs[ii-1].generate();
    }

    /* if tabid is currently selected, we need to set focus to a different tab */
    if (original_tab == tabIndex)
    {
        if (tabIndex == end-1)
            /* last one activates tab before removed tab */
            tabSelect(tabIndex-1);
        else
            /* activate tab after removed tab, which now has the same tabid */
            tabSelect(tabid);
    }
    else if (original_tab > tabIndex)
        /* Closed tab is to the left of the selected tab, which moved down 1 */
        tabSelect(original_tab-1);
    else
        gTabContainer.showTabs();
}

/* =======================================================================
    Close any tabs that are on a given connection because that connection
    has gone down.
*/
function closeDeadTabs(connection)
{
    var end = gTabObjs.length;
    var ii;

    for (ii = 1; ii < end; ++ii)
    {
        /* Don't close tabs associated with the main TFS */
        if (gTabObjs[0].connection.host == connection.host &&
                gTabObjs[0].connection.port == connection.port)
            continue;

        if (gTabObjs[ii].connection.host == connection.host &&
                gTabObjs[ii].connection.port == connection.port)
            tabCloseClick('CloseTab'+ii, ii);
    }
}


/* =======================================================================
    Keep inactive tabs alive so they don't log out.
    Tabs point to different servers and each server has a timeout limit.
    Called during refresh.  If refresh is off, let tabs log out.
*/
function keepTabsAlive()
{
    var actionstr = 'action=identity&';
    var selectedTab = getSelectedTab();
    var tab;
    var end = gTabObjs.length;
    var ii;

    for (ii = 0; ii < end; ++ii)
    {
        tab = gTabObjs[ii];
        if (!tab.connection || tab.tabtype != PAGETYPE_TFS ||
                (tab.connection.host == selectedTab.connection.host &&
                 tab.connection.port == selectedTab.connection.port))
            continue;

        requestAction(actionstr, ajaxKeepAlive, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, null);
        if (tab.connection.identity != '')
        {
            requestAction(actionstr, ajaxKeepAlive, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port) + MIRROR_PORT_OFFSET, null, null);
        }
    }
}


/* =======================================================================
    tabContainer method: Show only tabs that will fit on the page, starting at
    the 'leftmost' index.
*/
function showTabContainerTabs()
{
    var ii;
    var tab;
    var leftpos = TAB_START_LEFT_POS;
    var scrollend = 1;

    /* Adjust tab DIVs to be positioned correctly and set styles */
    for (ii = 0; ii < gTabObjs.length; ++ii)
    {
        tab = document.getElementById('pageTab'+ii);

        if (ii < this.leftmost)
        {
            tab.style.visibility = 'hidden';
            continue;
        }

        // Limit tabs to width of pageContainer
        if (scrollend == 0 || gTabObjs[ii].width + leftpos > this.width - TAB_PAGE_MARGIN)
        {
            tab.style.visibility = 'hidden';
            scrollend = 0;
            continue;
        }

        tab.style.width = gTabObjs[ii].width + "px";
        tab.style.visibility = 'inherit';
        gTabObjs[ii].leftpos = leftpos;
        tab.style.left = leftpos + "px";

        leftpos += gTabObjs[ii].width + TAB_SPACER_WIDTH;
    }

    setPlusTab(leftpos);

    addArrowTab(this.width);

    this.usedWidth = leftpos + gTabObjs['plus'].leftpos;

    document.body.style.cursor = 'auto';
}

/* =======================================================================
    tabContainerObj method: make sure tabid is visible
*/
function scrollTabContainer(tabid)
{
    var ii;
    var leftpos = TAB_START_LEFT_POS;
    var diff;

    if (tabid >= gTabObjs.length)
        return;

    /* Calculate used space upto this tabid */
    for (ii = 0; ii < tabid; ++ii)
        leftpos += gTabObjs[ii].width + TAB_SPACER_WIDTH;

    if (gTabObjs[ii].width + leftpos >= this.width - TAB_PAGE_MARGIN)
    {
        /* tabid won't fit, remove tabs until enough free space found */
        diff = (gTabObjs[ii].width + leftpos) - (this.width - TAB_PAGE_MARGIN);

        for (ii = 0; diff > 0 && ii < gTabObjs.length; ++ii)
        {
            diff -= gTabObjs[ii].width + TAB_SPACER_WIDTH;
        }

        this.leftmost = ii;
        setCookie('tabpage-system-lefttab', this.leftmost);
    }
    else if (ii < this.leftmost)
    {
        /* This tab fits, make sure it is not scrolled off to the left */
        this.leftmost = ii;
        setCookie('tabpage-system-lefttab', this.leftmost);
    }
}

/* =======================================================================
    tabContainerObj method: Create DIV for a new tab.
*/
function insertTabContainerTab(tabName)
{
    var container = document.getElementById('tabContainer');

    container.innerHTML = container.innerHTML + '<div id="'+tabName+'"></div>';
}

/* =======================================================================
    tabContainerObj method: Remove DIV for a tab.
*/
function removeTabContainerTab(tabName)
{
    var tab = document.getElementById(tabName);
    var tab_container = document.getElementById('tabContainer');

    /* Remove tabContainer child */
    if (tab)
        tab_container.removeChild(tab);

    if (this.leftmost)
    {
        this.leftmost--;
        setCookie('tabpage-system-lefttab', this.leftmost);
    }
}


/* =======================================================================
*/
function tabContainerObj()
{
    var container = document.getElementById('pageContainer');
    var pageWidth = container.clientWidth;
    var pageLeft = container.offsetLeft;
    var arrowWidth = gTabObjs['down'].width;

    this.leftmost = getCookie('tabpage-system-lefttab');
    if (this.leftmost == undefined)
        this.leftmost = 0;

    this.leftpos = pageLeft;
    this.width = pageLeft + pageWidth - arrowWidth - (2*TAB_SPACER_WIDTH);
    this.usedWidth = 0;

    this.showTabs = showTabContainerTabs;
    this.insert = insertTabContainerTab;
    this.remove = removeTabContainerTab;
    this.scrollToTab = scrollTabContainer;
}

/* =======================================================================
    If given tabname is already opened, select it and return true.
*/
function duplicateTabCheck(tabname)
{
    var lasttab = gTabObjs.length - 1;
    var ii;

    for (ii = 0; ii <= lasttab; ++ii)
    {
        if (gTabObjs[ii].getText().toLowerCase() == tabname.toLowerCase())
        {
            tabSelect(ii);
            reloadPage();
            return true;
        }
    }
    return false;
}

/* =======================================================================
    buttonObj method: Change button's HTML to be gray and unclickable
*/
function disableButton()
{
    var button;
    var html;
    var tblclass = ' class="button-table-disabled"';
    var txtclass = ' class="button-text-disabled"';

    if (this.image)
    {
        tblclass = '';
        txtclass = '';
    }
    html = '<center><table border="0" '+tblclass+'><tr><td '+txtclass+' onMouseOut="document.body.style.cursor=\'auto\'" >';

    if (this.image)
        html += '<img src="'+this.image+'" style="opacity:0.4;filter:alpha(opacity=40);">';
    else
        html += this.name;
    html += '</td></tr></table></center>';

    this.clicked = true;
    document.body.style.cursor = 'auto';

    button = document.getElementById(this.id);
    this.html = '<div id="'+this.id+'">' + html + '</div>';
    if (button)
        button.innerHTML = html;
}

/* =======================================================================
    buttonObj method: Make the button clickable.
*/
function enableButton()
{
    var button;
    var html;
    var ii;
    var tblclass = ' class="button-table"';
    var txtclass = ' class="button-text"';

    if (this.image)
    {
        tblclass = '';
        txtclass = '';
    }
    html = '<center><table border="0" '+tblclass+'><tr><td '+txtclass+' onMouseOver="document.body.style.cursor=\'pointer\'" onMouseOut="document.body.style.cursor=\'auto\'" onClick="'+this.fcn+'(\''+this.id+'\'';

    if (this.arg == null)
        html += ',\'\'';
    else
    {
        for (ii = 0; ii < this.arg.length; ++ii)
            html += ',\''+this.arg[ii]+'\'';
    }
    html += ');return false;">';

    if (this.image)
        html += '<img src="'+this.image+'" alt="'+this.name+'" title="'+this.name+'">';
    else
        html += this.name;
    html += '</td></tr></table></center>';

    this.clicked = false;

    button = document.getElementById(this.id);
    this.html = '<div id="'+this.id+'">' + html + '</div>';
    if (button)
        button.innerHTML = html;
}

/* =======================================================================
    buttonObj method: remove this button from the array.
*/
function destroyButton()
{
    gButtons[this.id] = undefined;
}

/* =======================================================================
    Button object constructor
*/
function buttonObj(buttonName, buttonID, buttonImage, buttonFcn, buttonArg)
{
    this.disableButton = disableButton;
    this.enableButton = enableButton;
    this.destroyButton = destroyButton;
    this.clicked = false;
    this.id = buttonID;
    this.name = buttonName;
    this.image = buttonImage;
    this.fcn = buttonFcn;
    this.arg = buttonArg;
    this.helptext = '';
}

/* =======================================================================
    Generate button in HTML.
    Returns the HTML, the caller must place the button on the page.

    buttonName is a string to be used as the button text.
    buttonID is the string to be used as the button id.
    buttonImage is an optional image to use instead of the buttonName on the
    face of the button.
    buttonFcn is a string of the function name (not function pointer)
    to call on a click.
    buttonArg is an array containing the arguments of the click function.

    Note: Javascript does not support associative arrays (those indexed by
    a string).  The 'index' is a property.
*/
function generateButton(buttonName, buttonID, buttonImage, buttonFcn, buttonArg)
{
    var button;

    if (gButtons[buttonID] == undefined)
    {
        button = gButtons[buttonID] = new buttonObj(buttonName, buttonID,
                                            buttonImage, buttonFcn, buttonArg);
        button.enableButton();
    }
    else
    {
        /* must reset arg when re-using a button, then regenerate html */
        button = gButtons[buttonID];
        button.arg = buttonArg;
        button.enableButton();
    }

    return button.html;
}

/* =======================================================================
    Remove the button based on its ID.
*/
function removeButtonById(buttonID)
{
    gButtons[buttonID].destroyButton();
}

/* =======================================================================
    Enable a button based on its ID.
*/
function enableButtonById(buttonID)
{
    gButtons[buttonID].enableButton();
}

/* =======================================================================
*/
function setButtonHelpText(buttonID, helptext)
{
    gButtons[buttonID].helptext = helptext;
}

/* =======================================================================
*/
function getButtonHelpText(buttonID)
{
    return gButtons[buttonID].helptext;
}

/* =======================================================================
    boxObj method: expand the contents of the box
*/
function expandBox(id)
{
    var box = getBox(id);
    var child;
    var container;
    var html = '<table border="0" width="100%"><tr><td width="99%"><img src="down_arrow.png" onMouseOver="document.body.style.cursor=\'pointer\'" onMouseOut="document.body.style.cursor=\'auto\'" onClick="collapseBox(\''+id+'\');return false;">';

    if (box == undefined)
        box = this;
    html += '<span class="box-title-text">'+box.title+'</span>';
    html += '</td><td align="right">'+box.helpbtn+'</td></tr></table>';
    document.getElementById(id+'_title').innerHTML = html;

    box.state = BOXSTATE_EXPANDED;

    if (box.children)
    {
        for (var ii = 0; ii < box.children.length; ++ii)
        {
            /* Firefox closes open drop downs if .display is changed here */
            if (gBrowserIE)
                document.getElementById('parent'+id+'_'+ii).style.display = 'inline';
            /* Init internal containers as hidden or visible */
            child = document.getElementById(id+'_'+box.children[ii]);
            child.style.visibility = 'inherit';
            child.style.height = 'auto';
            child.style.display = 'inline';
        }
    }
    else
    {
        container = document.getElementById(id+'_content');
        container.style.display = 'inline';
        container.innerHTML = box.content;
    }
}

/* =======================================================================
    boxObj method: collapse the contents of the box
*/
function collapseBox(id)
{
    var box = getBox(id);
    var child;
    var container;
    var html = '<table border="0" width="100%"><tr><td width="99%"><img src="right_arrow.png" onMouseOver="document.body.style.cursor=\'pointer\'" onMouseOut="document.body.style.cursor=\'auto\'" onClick="expandBox(\''+id+'\');return false;">';

    if (box == undefined)
        box = this;
    html += '<span class="box-title-text">'+box.title+'</span>';
    html += '</td><td align="right">'+box.helpbtn+'</td></tr></table>';

    document.getElementById(id+'_title').innerHTML = html;

    box.state = BOXSTATE_COLLAPSED;

    if (box.children)
    {
        for (var ii = 0; ii < box.children.length; ++ii)
        {
            /* Firefox closes open drop downs if .display is changed here */
            if (gBrowserIE)
                document.getElementById('parent'+id+'_'+ii).style.display = 'none';
            /* Init internal containers as hidden or visible */
            child = document.getElementById(id+'_'+box.children[ii]);
            child.style.visibility = 'hidden';
            child.style.height = "0px";
            child.style.display = 'none';
        }
    }
    else
    {
        container = document.getElementById(id+'_content');
        container.style.display = 'none';
        container.innerHTML = '';
    }
}

/* =======================================================================
    boxObj method: replace the contents of the box
*/
function updateBoxContent(content)
{
    var div = document.getElementById(this.id+'_content');

    this.content = content;
    div.innerHTML = content;
}

/* =======================================================================
    boxObj method: replace the contents of the box
    'id' is the id of the child's div, which was the string in the 'children'
    array passed to the boxObj constructor.
*/
function updateBoxChildContent(id, content)
{
    var child = document.getElementById(this.id+'_'+id);

    child.innerHTML = content;
}

/* =======================================================================
    boxObj method: replace the box's title string
*/
function setBoxTitle(title)
{
    this.title = title;

    if (this.state == BOXSTATE_EXPANDED)
        this.expand(this.id);
    else
        this.collapse(this.id);
}

/* =======================================================================
    boxObj method: Call refresh function to refresh box content, which will do
    requestActions.
*/
function refreshBox()
{
    if (this.refreshfcn)
        this.refreshfcn();
}

/* =======================================================================
    Display box object.  This box has an arrow that allows for expanding
    and collapsing of the contents.

    'children' is an array of containers to be placed inside this box.

    'helptext' is optional.  If it exists a help button is put in the box.

    'refreshfcn' is optional.  If it exists, it will be called during every refresh.

    Caller must create <div id> of the given 'id' and output that so the element
    exists in the document, before calling new boxObj.

    This object will add a 'id_title' div to hold the title.
    This object will add a 'id_content' div to hold content if there are no
    children.
    It will also add divs for any children passed in.  The children divs cannot
    be overwritten everytime we switch states, otherwise there is a lot of
    flashing.
*/
function boxObj(id, title, state, children, helptext, refreshfcn)
{
    var div = document.getElementById(id);
    var content;
    var container;
    var btnarg = new Array();

    btnarg[0] = id;

    this.id = id;
    this.title = title;
    this.state = state; //BOXSTATE_EXPANDED | BOXSTATE_COLLAPSED
    this.children = children;
    this.refreshfcn = refreshfcn;
    this.content = '';
    this.helpbtn = '';
    if (helptext != undefined && helptext != null && helptext != '')
    {
        this.helpbtn = generateButton('Help', 'Help'+id, 'question.png',
                                      'helpClick', btnarg);
        setButtonHelpText('Help'+id, helptext);
    }

    this.expand = expandBox;
    this.collapse = collapseBox;
    this.update = updateBoxContent;
    this.updateChild = updateBoxChildContent;
    this.setTitle = setBoxTitle;
    this.refresh = refreshBox;

    content = '<div id="'+id+'_title"></div><div id="'+id+'_content"></div>';

    if (children)
    {
        /* Firefox needs a parent div wrapped around each child or drop down
           will close when other ajax parts refresh
         */
        for (var ii = 0; ii < children.length; ++ii)
            content += '<div id="parent'+id+'_'+ii+'"><div id="'+id+'_'+children[ii]+'"></div></div>';
    }

    div.style.visibility = 'inherit';

    div.innerHTML = content;

    document.getElementById(id+'_title').style.visibility = 'inherit';
    document.getElementById(id+'_content').style.visibility = 'inherit';
    if (children)
    {
        for (var ii = 0; ii < children.length; ++ii)
        {
            document.getElementById('parent'+id+'_'+ii).style.visibility = 'inherit';
            container = document.getElementById(id+'_'+children[ii]);
            container.style.visibility = 'inherit';
        }
    }

    if (state == BOXSTATE_EXPANDED)
        this.expand(id);
    else if (state == BOXSTATE_COLLAPSED)
        this.collapse(id);

    container = document.getElementById(id);
    container.style.width = (getMainTableWidth() - (2 * getMainTableMargin()) - (2 * getPageMargin()) - BOUNDARY_BOX_MARGIN) + "px";
    container.style.left = BOUNDARY_BOX_MARGIN + "px";
}

/* =======================================================================
    We keep box objects in a global array because we need to access them
    from html links where we don't have access to 'this'.

    Returns the box object.
*/
function generateBox(id, title, state, children, helptext, refreshfcn)
{
    var newid = gBoxes.length;
    var box;

    if ((box=getBox(id)) == undefined)
        gBoxes[newid] = new boxObj(id, title, state, children, helptext, refreshfcn);
    else
        return box;

    return gBoxes[newid];
}

/* =======================================================================
    Retrieve the box object from an string id.
*/
function getBox(id)
{
    for (var ii = 0; ii < gBoxes.length; ++ii)
    {
        if (gBoxes[ii].id == id)
            break;
    }

    return gBoxes[ii];
}

/* =======================================================================
    Field object, one for each field in the schema, stored in the recordObj.
*/
function fieldObj(fieldname, fdrecno, fdkey, fdtype, dim0, dim1, dim2, flags, keyfile, extfile)
{
    this.fieldname = fieldname;
    this.fdrec = fdrecno;
    this.fdkey = fdkey;
    this.fdtype = fdtype;
    this.dim0 = dim0;
    this.dim1 = dim1;
    this.dim2 = dim2;
    this.flags = flags;
    this.keyfile = keyfile;
    this.extfile = extfile;

    this.getFieldName = function () { return this.fieldname; };
    this.getFieldRecno = function () { return this.fdrec; };
    this.getFieldType = function () { return this.fdtype; };
    this.getFieldDim0 = function () { return this.dim0; };
    this.getFieldDim1 = function () { return this.dim1; };
    this.getFieldDim2 = function () { return this.dim2; };
    this.getFieldFlags = function () { return this.flags; };
    this.getFieldKey = function () { return this.fdkey; };
    this.getFieldKeyFile = function () { return this.keyfile; };
    this.getFieldExtFile = function () { return this.extfile; };
}

/* =======================================================================
    Compound key record object
*/
function keyFieldObj(keyfield, compkey, keysort)
{
    this.keyfield = parseInt(keyfield);  // field_entry index 
    this.compkey = parseInt(compkey);    // component key field_entry index
    this.keysort = keysort; 
    
    this.getKeyField = function () { return this.keyfield; };
    this.getCompKey = function () { return this.compkey; };
    this.getKeySort = function () { return this.keysort; };
}


/* =======================================================================
    Record object, one for each record in the schema.
*/
function recordObj(recname, rfile, rflags, rfdtot, rfields)
{
    this.recname = recname;
    this.file = rfile;
    this.fdtot = rfdtot;
    this.fields = rfields;      // index into fields of first field
    this.keyfields = new Array();
    this.flags = rflags;

    this.getFile = function () { return this.file; };
    this.addKeyField = function (kfield) { this.keyfields.push(kfield); };
    this.getRecordName = function () { return this.recname; };
    this.getField = function () { return this.fields; };
    this.getNumFields = function () { return this.fdtot; };
    this.getRecordFlags = function () { return this.flags; };
    this.getNumKeyFields = function () { return this.keyfields.length; };
    this.getKeyField = function (idx) { return this.keyfields[idx] };
}


/* =======================================================================
    Sort entry object
*/
function sortObj(field, set)
{
    this.field = field;
    this.set = set;
    
    this.getField = function () { return this.field; };
    this.getSet = function () { return this.set; };
}

/* =======================================================================
    Member object
*/
function memberObj(record)
{
    this.record = record;
    this.sorts = new Array();
    
    this.getRecord = function () { return this.record; };
    this.addSortField = function(srtobj) { this.sorts.push(srtobj); };
    this.getNumSortFields = function () { return this.sorts.length; };
    this.getSortField = function(idx) { return this.sorts[idx]; };
}

/* =======================================================================
    Set object
*/
function setObj(setname, setorder, setowner)
{
    this.name = setname;
    this.order = setorder;
    this.owner = setowner;
    this.members = new Array();
    
    this.getSetName = function () { return this.name; };
    this.getOrder = function () { return this.order; };
    this.getOwner = function () { return this.owner; };
    this.addMember = function(mem) { this.members.push(mem); };
    this.getNumMembers = function () { return this.members.length; };
    this.getMember = function(idx) { return this.members[idx]; };
}

/* =======================================================================
    File object
*/
function fileObj(filename, ftype, fslots, fslsize, fpgsize)
{
    this.name = filename;
    this.type = ftype;
    this.slots = fslots;
    this.slsize = fslsize;
    this.pgsize = fpgsize;
    this.fields = new Array();  // array of fields that are in a file separate 
                                // from record (keys, vardata and blobs)
    
    this.getFileName = function () { return this.name; };
    this.getFileType = function () { return this.type; };
    this.getSlots = function () { return this.slots; };
    this.getSlotSize = function () { return this.slsize; };
    this.getPageSize = function () { return this.pgsize; };
    this.addField = function (idx) { this.fields.push(idx); };
    this.getNumFields = function () { return this.fields.length; };
    this.getField = function (idx) { return this.fields[idx]; };
}

/* =======================================================================
    Query mirroring server's identity (DBMIRROR, DBREP, DBREPSQL).
*/
function getIdentity()
{
    var actionstr = 'action=identity&';
    var tab = getSelectedTab();

    if (tab && tab.connection)
        requestAction(actionstr, ajaxIdentity, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port) + MIRROR_PORT_OFFSET, null, null);
}

/* =======================================================================
*/
function ajaxIdentity(req)
{
    var response;
    var displayData = new Array();
    /* Names returned by tfs */
    var rdmNames = new Array(
        'Identity'
    );

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 1, req.tab.connection)) == '')
            return;

        if (response == ' ')
        {
            req.tab.connection.identity = "";
            return;
        }

        // split out name/value pairs
        parseResponse(response, displayData, rdmNames[0]);

        req.tab.connection.identity = displayData[0][0];
        reloadPage();
    }
}

/* =======================================================================
    Do nothing but validate the login.
*/
function ajaxKeepAlive(req)
{
    if (req.readyState == 4 /* complete */ && req.status == 200)
        validateLogin(req.responseText, 1, req.tab.connection);
}

/* =======================================================================
    Rename the main tab if its host name was localhost.
*/
function ajaxGetHostName(req)
{
    var response;
    var displayData = new Array();
    /* Names returned by tfs */
    var rdmNames = new Array(
        'Hostname'
    );

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 0, req.tab.connection)) == '')
            return;

        if (response == ' ')
            return;

        // split out name/value pairs
        parseResponse(response, displayData, rdmNames[0]);

        req.tab.connection.host = displayData[0][0];
        renameTab(req.tab.tabid, displayData[0][0]+':'+req.tab.connection.port);
    }
}

/* =======================================================================
*/
function ajaxUserDisconnect(req)
{
    var response;
    var displayData = new Array();
    /* Names returned by tfs */
    var rdmNames = new Array(
        'User',
        'rc'
    );

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 0, req.tab.connection)) == '')
            return;

        // split out name/value pairs
        if (parseResponse(response, displayData, rdmNames[0]) == 0)
            return;

        if (displayData[0][1] != 0)
            alert('Unable to remove user '+ displayData[0][0] +', rc = ' + rc);
        removeButtonById(req.userdata);

        if (getRefreshRate() == 0)
            setTimeout(reloadPage, 2000);
    }
}

/* =======================================================================
    Change \ to / in dbnames.
    If a dbname has a slash, it is a slave database.
*/
function normalizeDbName(dbname)
{
    return dbname.replace(/\\/g, '/');
}

/* =======================================================================
*/
function parseDbNameResponse(responseText, connection, proxy)
{
    var tab;
    var response;
    var dbhtml = '';
    var displayData = new Array();
    /* Names returned by tfs */
    var rdmNames = new Array(
        'DbName',
        'Mirroring',
        'Replication'
    );

    if ((response=validateLogin(responseText, proxy, connection)) == '')
        return false;

    connection.dbobjs = new Array();

    if (response.substr(0, 7) == 'Warning')
    {
        if (proxy)
            return false;
        alert('Warning: '+getErrorString(response.substr(8)));
        return false;
    }

    // split out name/value pairs
    parseResponse(response, displayData, rdmNames[0]);

    for (var ii = 0; ii < displayData.length; ++ii)
    {
        if (displayData[ii][0] == "")
            continue;

        connection.dbobjs[ii] = new databaseObj(displayData[ii][0],
                                                displayData[ii][1],
                                                displayData[ii][2]);
    }

    /* Make sure old cookies are not out of bounds if we changed servers */
    tab = getCookie('tabpage-databases');
    if (tab >= connection.dbobjs.length)
    {
        setCookie('tabpage-databases', 0);
    }

    return true;
}

/* =======================================================================
*/
function ajaxGetDbNameResponseHandler(req)
{
    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        parseDbNameResponse(req.responseText, req.tab.connection, 0);
        reloadPage();
    }
}

/* =======================================================================
*/
function ajaxConnectResponseHandler(req)
{
    var proxy = gLoginRetry == 0 ? 1 : 0; //Prevent error before 1st retry

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if (parseDbNameResponse(req.responseText, req.tab.connection, proxy))
        {
            renameTab(req.tab.tabid,
                        req.tab.connection.host+':'+req.tab.connection.port);
            setCurrentTabType(PAGETYPE_TFS);
            setPageType(PAGETYPE_TFS);

            resumeRefresh();
            reloadPage();

            updateLocalhostName(req.tab.connection);

            gLoginRetry = 0;
            gButtons['Connect'].enableButton();
        }
        else if (gLoginRetry < 1 && getCurrentTabType() == PAGETYPE_CONNECT)
        {
            /* The remote server timed out, retry once */
            connectClick(req.userdata);
            gLoginRetry++;
        }
        else
        {
            gButtons['Connect'].enableButton();
            gLoginRetry = 0;
        }
    }
}


/* =======================================================================
    Get database names for all databases on the server, whether or not
    they are opened.
*/
function getDbNames()
{
    var actionstr = "action=dblist";
    var tab = getSelectedTab();

    requestAction(actionstr, ajaxGetDbNameResponseHandler, 'proxy.rdm',
                    tab.connection.host, tab.connection.port, null, null);
    return 0;
}

/* =======================================================================
    Display function for User information
    We don't display all the information returned from the server.
*/
function ajaxUsersResponseHandler(req)
{
    var response;
    var userhtml;
    var numusers = 0;
    var container;
    var mywidth;
    var box;
    var buttonarg = new Array();
    var displayData = new Array();
    /* Names returned by tfs */
    var rdmNames = new Array(
        'User',
        ''
    );

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 0, req.tab.connection)) == '')
            return;

        // split out name/value pairs
        numusers = parseResponse(response, displayData, rdmNames[0]);

        /* Build user info */
        container = document.getElementById('UsersContainer');
        mywidth = container.offsetWidth-BOUNDARY_BOX_INDENT;

        userhtml = '<table border="0">';

        if (numusers > 0)
        {
            for (var ii = 0; ii < displayData.length; ++ii)
            {
                if (displayData[ii][0] == "")
                    continue;

                //Data row(s)
                userhtml += '<tr><td width="'+BOUNDARY_BOX_INDENT+'">&nbsp;</td>';
                userhtml += '<td class="box-text">'+ displayData[ii][0] +'</td>';
                /* insert button container */
                userhtml += '<td width="'+BOUNDARY_BOX_INDENT+'">&nbsp;</td><td>';
                buttonarg[0] = displayData[ii][0];
                userhtml += generateButton('Remove', 'Remove'+ii, 'x12r.png',
                                    'disconnectUserClick', buttonarg);
                userhtml += '</td></tr>';
            }
        }
        else
        {
            // No users, display an empty row
            userhtml += '<tr><td width="'+BOUNDARY_BOX_INDENT+'">&nbsp;</td><td class="table-text-1" colspan="2"><center>No users connected</center></td></tr>';
        }

        userhtml += '</table>';

        box = getBox('UsersContainer');
        box.update(userhtml);
    }
}

/* =======================================================================
    function to receive INI settings
*/
function ajaxINIGetHandler(req)
{
    var response;
    var numvalues;
    var keystr;
    var unit;
    var idx;
    var idPrefix = '';
    var displayData = new Array();
    /* Names returned by tfs */
    var rdmNames = new Array(
        'Name',
        'Value'
    );

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 0, req.tab.connection)) == '')
            return;

        if (req.userdata != '')
            idPrefix = 'db';

        // split out name/value pair
        numvalues = parseResponse(response, displayData, rdmNames[0]);

        if (numvalues > 0)
        {
            keystr = displayData[0][0].toLowerCase();
            if (keystr == 'mirroring')
            {
                if (displayData[0][1] != '0')
                    document.getElementById(idPrefix+'SettingsMirLog').checked = true;
            }
            else if (keystr == 'replication')
            {
                if (displayData[0][1] != '0')
                    document.getElementById(idPrefix+'SettingsRepLog').checked = true;
            }
            else if (keystr == 'singleslavemode' && displayData[0][1] == '0')
                document.getElementById(idPrefix+'SettingsSingleSlave').checked = false;
            else if (keystr == 'logfileage' && displayData[0][1] != '')
            {
                unit = displayData[0][1].match(/s|m|h|d/i);
                switch (unit.toString())
                {
                    case 's':
                        idx = 0;
                        break;
                    case 'm':
                        idx = 1;
                        break;
                    case 'h':
                        idx = 2;
                        break;
                    case 'd':
                        idx = 3;
                        break;
                    default:
                        idx = 1; // default to mins
                        break;
                }
                document.getElementById(idPrefix+'FlowLogAge').value = parseInt(displayData[0][1]);
                document.getElementById(idPrefix+'SettingsMirLogAge').selectedIndex = idx;
            }
            else if (keystr == 'logfilespace' && displayData[0][1] != '')
            {
                unit = displayData[0][1].match(/k|m|g/i);
                switch (unit.toString())
                {
                    case 'k':
                        idx = 0;
                        break;
                    case 'm':
                        idx = 1;
                        break;
                    case 'g':
                        idx = 2;
                        break;
                    default:
                        idx = 0; // default to Kb
                        break;
                }
                document.getElementById(idPrefix+'FlowLogSpace').value = parseInt(displayData[0][1]);
                document.getElementById(idPrefix+'SettingsMirLogSpace').selectedIndex = idx;
            }
        }
    }
}

/* =======================================================================
    function to set INI settings
*/
function ajaxINISetHandler(req)
{
    var response;
    var userhtml;
    var numusers = 0;
    var container;
    var displayData = new Array();
    /* Names returned by tfs */
    var rdmNames = new Array(
        'Name',
        'Value'
    );

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 0, req.tab.connection)) == '')
            return;

        // split out name/value pair
        numusers = parseResponse(response, displayData, rdmNames[0]);

        if (req.userdata)
        {
            /* set a timer to enable the button since the update happens so
               fast there is no time for the button to go gray.
             */
            /* get dbnames, which also regets db's ini settings for mir/rep */
            setTimeout(function() {enableButtonById(req.userdata);getDbNames()}, 750);
        }
    }
}

/* =======================================================================
    function to remove database specific ini file on server
*/
function ajaxINIRevertHandler(req)
{
    var response;
    var userhtml;
    var numusers = 0;
    var container;
    var displayData = new Array();
    /* Names returned by tfs */
    var rdmNames = new Array(
        'Result'
    );

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 0, req.tab.connection)) == '')
            return;

        // split out name/value pair
        numusers = parseResponse(response, displayData, rdmNames[0]);

        if (req.userdata)
        {
            /* set a timer to enable the button since the update happens so
               fast there is no time for the button to go gray.
             */
            /* get dbnames, which also regets db's ini settings for mir/rep */
            setTimeout(function() {enableButtonById(req.userdata);getDbNames();reloadPage();}, 750);
        }
    }
}

/* =======================================================================
    Parse XML field_entry data and create 'fields' array.
*/
function buildFieldObjs(xmlDoc, total_fields, files)
{
    var ii;
    var fd;
    var fields = new Array();
    var fieldname;
    var fdrecno;
    var fdkey;
    var fdtype;
    var fd0, fd1, fd2;
    var fdflags;
    var keyfile;
    var extfile;
    
    fd = xmlDoc.getElementsByTagName("field_entry");
    
    /* Build fieldObjs */
    for (ii = 0; ii < total_fields; ++ii)
    {
        fieldname = fd[ii].getElementsByTagName("name")[0].childNodes[0].nodeValue;
        fdrecno = fd[ii].getElementsByTagName("rec")[0].childNodes[0].nodeValue;
        fdkey = fd[ii].getElementsByTagName("key")[0].childNodes[0].nodeValue;
        fdtype = fd[ii].getElementsByTagName("type")[0].childNodes[0].nodeValue;
        fd0 = fd[ii].getElementsByTagName("dim0")[0].childNodes[0].nodeValue;
        fd1 = fd[ii].getElementsByTagName("dim1")[0].childNodes[0].nodeValue;
        fd2 = fd[ii].getElementsByTagName("dim2")[0].childNodes[0].nodeValue;
        fdflags = fd[ii].getElementsByTagName("flags")[0].childNodes[0].nodeValue;
        keyfile = fd[ii].getElementsByTagName("keyfile")[0].childNodes[0].nodeValue;
        extfile = fd[ii].getElementsByTagName("extfile")[0].childNodes[0].nodeValue;

        fields[ii] = new fieldObj(fieldname, fdrecno, fdkey, fdtype,
                                    fd0, fd1, fd2, fdflags, keyfile, extfile);

        if (fdkey != 'n')
            files[keyfile].addField(ii);
        if (fdtype == 'b' || fdtype == 'v')
            files[extfile].addField(ii);
    }
    
    return fields;
}

/* =======================================================================
    Parse XML key_entry data and create 'keys' array.
*/
function buildKeyObjs(xmlDoc, total_key_fields)
{
    var ii;
    var kf;
    var keys = new Array();
    var keyfield;
    var compkey;
    var keysort;
    
    kf = xmlDoc.getElementsByTagName("key_entry");
    
    /* build keyObjs */
    for (ii = 0; ii < total_key_fields; ++ii)
    {
        keyfield = kf[ii].getElementsByTagName("key")[0].childNodes[0].nodeValue;
        compkey = kf[ii].getElementsByTagName("field")[0].childNodes[0].nodeValue;
        keysort = kf[ii].getElementsByTagName("sort")[0].childNodes[0].nodeValue;
        
        keys[ii] = new keyFieldObj(keyfield, compkey, keysort);
    }

    return keys;
}

/* =======================================================================
    Parse XML record_entry data and create 'records' array.
*/
function buildRecordObjs(xmlDoc, total_records, fields, keys, total_key_fields)
{
    var ii, jj;
    var rt;
    var records = new Array();
    var recname;
    var rfile;
    var rfields;
    var rfdtot;
    var rflags;
    
    rt = xmlDoc.getElementsByTagName("record_entry");

    /* build recordObjs and insert correct fieldObjs for each record */
    for (ii = 0; ii < total_records; ++ii)
    {
        recname = rt[ii].getElementsByTagName("name")[0].childNodes[0].nodeValue;
        rfile = parseInt(rt[ii].getElementsByTagName("file")[0].childNodes[0].nodeValue);
        rfields = parseInt(rt[ii].getElementsByTagName("fields")[0].childNodes[0].nodeValue);
        rfdtot = parseInt(rt[ii].getElementsByTagName("fdtot")[0].childNodes[0].nodeValue);
        rflags = parseInt(rt[ii].getElementsByTagName("flags")[0].childNodes[0].nodeValue);

        records[ii] = new recordObj(recname, rfile, rflags, rfdtot, rfields);

        if (rfdtot == 65535)
            continue;

        /* check if any key fields are part of this record */
        for (jj = 0; jj < total_key_fields; ++jj)
        {
            if (fields[keys[jj].getCompKey()].getFieldRecno() == ii)
                records[ii].addKeyField(keys[jj]);
        }
    }
    
    return records;
}

/* =======================================================================
    Parse XML sort_entry data and create 'sorts' array.
*/
function buildSortObjs(xmlDoc, total_sort_fields)
{
    var ii;
    var sr;
    var sorts = new Array();
    var field;
    var set;
    
    sr = xmlDoc.getElementsByTagName("sort_entry");
    
    /* build sortObjs */
    for (ii = 0; ii < total_sort_fields; ++ii)
    {
        field = sr[ii].getElementsByTagName("field")[0].childNodes[0].nodeValue;
        set = sr[ii].getElementsByTagName("set")[0].childNodes[0].nodeValue;
        
        sorts[ii] = new sortObj(field, set);
    }
    
    return sorts;
}

/* =======================================================================
    Parse XML member_entry data and create 'members' array.
*/
function buildMemberObjs(xmlDoc, total_members, sorts)
{
    var ii, jj;
    var mr;
    var members = new Array();
    var record;
    var sort_fld;
    var totsf;
    
    mr = xmlDoc.getElementsByTagName("member_entry");
    
    /* build memberObjs */
    for (ii = 0; ii < total_members; ++ii)
    {
        record = mr[ii].getElementsByTagName("record")[0].childNodes[0].nodeValue;
        sort_fld = parseInt(mr[ii].getElementsByTagName("sort_fld")[0].childNodes[0].nodeValue);
        totsf = parseInt(mr[ii].getElementsByTagName("totsf")[0].childNodes[0].nodeValue);

        members[ii] = new memberObj(record);

        for (jj = 0; jj < totsf; ++jj)
            members[ii].addSortField(sorts[sort_fld+jj]);
    }
    
    return members;
}

/* =======================================================================
    Parse XML set_entry data and create 'sets' array.
*/
function buildSetObjs(xmlDoc, total_sets, members)
{
    var ii, jj;
    var st;
    var sets = new Array();
    var setname;
    var setorder;
    var setowner;
    var setmember;
    var setmemtot;
    
    st = xmlDoc.getElementsByTagName("set_entry");
    
    /* build setObjs */
    for (ii = 0; ii < total_sets; ++ii)
    {
        setname = st[ii].getElementsByTagName("name")[0].childNodes[0].nodeValue;
        setorder = st[ii].getElementsByTagName("order")[0].childNodes[0].nodeValue;
        setowner = st[ii].getElementsByTagName("own_rt")[0].childNodes[0].nodeValue;
        setmember = parseInt(st[ii].getElementsByTagName("members")[0].childNodes[0].nodeValue);
        setmemtot = parseInt(st[ii].getElementsByTagName("memtot")[0].childNodes[0].nodeValue);
        
        sets[ii] = new setObj(setname, setorder, setowner);
        
        for (jj = 0; jj < setmemtot; ++jj)
            sets[ii].addMember(members[setmember+jj]);
    }
    
    return sets;
}


/* =======================================================================
    Parse XML file_entry data and create 'files' array.
*/
function buildFileObjs(xmlDoc, total_files)
{
    var ii;
    var ft;
    var files = new Array();
    var filename;
    var ftype;
    var fslots;
    var fslsize;
    var fpgsize;
    
    ft = xmlDoc.getElementsByTagName("file_entry");
    
    /* build fileObjs */
    for (ii = 0; ii < total_files; ++ii)
    {
        filename = ft[ii].getElementsByTagName("name")[0].childNodes[0].nodeValue;
        ftype = ft[ii].getElementsByTagName("type")[0].childNodes[0].nodeValue;
        fslots = ft[ii].getElementsByTagName("slots")[0].childNodes[0].nodeValue;
        fslsize = ft[ii].getElementsByTagName("slsize")[0].childNodes[0].nodeValue;
        fpgsize = ft[ii].getElementsByTagName("pgsize")[0].childNodes[0].nodeValue;
        
        files[ii] = new fileObj(filename, ftype, fslots, fslsize, fpgsize);
    }
    
    return files;
}


/* =======================================================================
    Parse the xml file into an array of objects for ease of display on refresh.
    Only save the xml data we will use.
*/
function parseSchemaXML(xml)
{
    var parser;
    var xmlDoc;
    var tab = getSelectedTab();
    var tabid = getCookie('tabpage-databases');
    var total_records;
    var total_fields;
    var total_key_fields;
    var total_sets;
    var total_members;
    var total_sort_fields;
    var total_files;
    var records;
    var fields;
    var keys;
    var sets;
    var sorts;
    var members;
    var files;

    if (window.DOMParser)
    {
        parser = new DOMParser();
        xmlDoc = parser.parseFromString(xml, "text/xml");
    }
    else // Internet Explorer
    {
        xmlDoc = new ActiveXObject("Microsoft.XMLDOM");
        xmlDoc.async = "false";
        xmlDoc.loadXML(xml);
    }

    total_records = parseInt(xmlDoc.getElementsByTagName("total_records")[0].childNodes[0].nodeValue);
    total_files = parseInt(xmlDoc.getElementsByTagName("total_files")[0].childNodes[0].nodeValue);
    total_fields = parseInt(xmlDoc.getElementsByTagName("total_fields")[0].childNodes[0].nodeValue);
    total_key_fields = parseInt(xmlDoc.getElementsByTagName("total_key_fields")[0].childNodes[0].nodeValue);
    total_sets = parseInt(xmlDoc.getElementsByTagName("total_sets")[0].childNodes[0].nodeValue);
    total_members = parseInt(xmlDoc.getElementsByTagName("total_members")[0].childNodes[0].nodeValue);
    total_sort_fields = parseInt(xmlDoc.getElementsByTagName("total_sort_fields")[0].childNodes[0].nodeValue);

    files = buildFileObjs(xmlDoc, total_files);

    fields = buildFieldObjs(xmlDoc, total_fields, files);
    keys = buildKeyObjs(xmlDoc, total_key_fields);
    records = buildRecordObjs(xmlDoc, total_records, fields, keys, total_key_fields);
    
    sorts = buildSortObjs(xmlDoc, total_sort_fields);
    members = buildMemberObjs(xmlDoc, total_members, sorts);
    sets = buildSetObjs(xmlDoc, total_sets, members);
    
    tab.connection.dbobjs[tabid].records = records;
    tab.connection.dbobjs[tabid].sets = sets;
    tab.connection.dbobjs[tabid].fields = fields;
    tab.connection.dbobjs[tabid].files = files;
}

/* =======================================================================
*/
function ajaxSchemaResponseHandler(req)
{
    var response;

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 0, req.tab.connection)) == '')
            return;

        if (response != null && response.trim() != '')
            parseSchemaXML(response);

        displaySchemaContainer();
    }
}

/* =======================================================================
*/
function ajaxDatabasesLocksHandler(req)
{
    var container;
    var mywidth;
    var box;
    var response;
    var dbhtml;
    var numfiles = 0;
    var displayData = new Array();
    /* Names returned by tfs */
    var rdmNames = new Array(
        'File',
        'LockStat',
        'NumLocks',
        'HoldingLock',
        'Waiting'
    );

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 0, req.tab.connection)) == '')
            return;

        container = document.getElementById('DbLocksContainer');
        mywidth = container.offsetWidth-BOUNDARY_BOX_INDENT;

        dbhtml = '<center><table border="0" width="'+mywidth+'">';

        numfiles = parseResponse(response, displayData, rdmNames[0]);

        //Title row
        dbhtml += '<tr><td class="box-subtitle-text" width="175">File</td><td class="box-subtitle-text">Lock State</td><td class="box-subtitle-text">Number of Locks</td><td class="box-subtitle-text">User With Lock</td><td class="box-subtitle-text">Users Waiting</td></tr>';

        if (numfiles > 0)
        {
            dbhtml += buildDisplayRows(displayData, rdmNames, null, null, 0);
        }
        else
        {
            // No dbs open, display an empty row
            dbhtml += '<tr><td class="table-text-1" colspan="5"><center>Database not open</center></td></tr>';
        }

        dbhtml += '</table>';
        dbhtml += '<br><br>';

        dbhtml += '</center>';

        box = getBox('DbLocksContainer');
        box.update(dbhtml);
    }
}

/* =======================================================================
*/
function ajaxDatabasesUsersHandler(req)
{
    var container;
    var mywidth;
    var box;
    var response;
    var dbhtml;
    var numusers = 0;
    var displayData = new Array();
    /* Names returned by tfs */
    var rdmNames = new Array(
        'User',
        'LastTxid'
    );

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 0, req.tab.connection)) == '')
            return;

        container = document.getElementById('DbUsersContainer');
        mywidth = container.offsetWidth-BOUNDARY_BOX_INDENT;

        dbhtml = '<center><table border="0" width="'+mywidth+'">';

        numusers = parseResponse(response, displayData, rdmNames[0]);

        //Title row
        dbhtml += '<tr><td class="box-subtitle-text" width="175">User</td><td class="box-subtitle-text">LastTxid</td></tr>';

        if (numusers > 0)
        {
            dbhtml += buildDisplayRows(displayData, rdmNames, null, null, 0);
        }
        else
        {
            // No dbs open, display an empty row
            dbhtml += '<tr><td class="table-text-1" colspan="2"><center>Database not open</center></td></tr>';
        }

        dbhtml += '</table>';
        dbhtml += '<br><br>';

        dbhtml += '</center>';

        box = getBox('DbUsersContainer');
        box.update(dbhtml);
    }
}



/* =======================================================================
*/
function ajaxMasterFlowListHandler(req)
{
    var response;
    var mirhtml;
    var numslaves;
    var mywidth;
    var container;
    var box;
    var btnargs = new Array();
    var dbnameIdx = 0; // Index of DbName in rdmNames
    var slhostIdx = 1;
    var slportIdx = 2;
    var typeIdx = 3;   // Index of Type in rdmNames
    var statusIdx = 4; // Index of Status in rdmNames
    var syncIdx = 5;   // Index of Sync in rdmNames
    var txidIdx = 6;   // Index of last txid in rdmNames
    var odbcIdx = 7;   // Index of ODBC in rdmNames
    var displayData = new Array();
    /* Names returned by dbmirror */
    var rdmNames = new Array(
        'DbName',
        'SlaveHost',
        'SlavePort',
        'Type',
        'Status',
        'Sync',
        'LastTxid',
        'ODBC',
        ''
    );
    var status_str = new Array (
        'Starting',
        'Not-Ready',
        'Ready',
        'Not-Running'
    );

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 1, req.tab.connection)) == '')
            return;

        numslaves = parseResponse(response, displayData, rdmNames[0]);

        for (var ii = 0; ii < numslaves; ++ii)
        {
            /* status is returned as a number, convert to a string */
            displayData[ii][statusIdx] = status_str[displayData[ii][statusIdx]];
            displayData[ii][typeIdx] = getMirServerType(displayData[ii][typeIdx],
                                            displayData[ii][syncIdx],
                                            displayData[ii][odbcIdx]);
        }

        container = document.getElementById('DataflowContainer');
        mywidth = container.offsetWidth-BOUNDARY_BOX_INDENT;

        mirhtml = '<br><center><table border="0" width="'+mywidth+'">';

        //Title row
        mirhtml += '<tr><td class="box-subtitle-text-with-bkgrnd">Master of</td><td class="box-subtitle-text-with-bkgrnd">Type</td><td class="box-subtitle-text-with-bkgrnd">Status</td><td class="box-subtitle-text-with-bkgrnd">Txid</td><td class="box-subtitle-text-with-bkgrnd">Action</td></tr>';

        if (numslaves > 0)
        {
            for (var ii = 0; ii < displayData.length; ++ii)
            {
                if (displayData[ii][0] == "")
                    continue;

                //Data row(s)
                mirhtml += '<tr>';
                mirhtml += '<td class="table-text-2" colspan="5">'+
                            displayData[ii][dbnameIdx] + '@' +
                            displayData[ii][slhostIdx] + ':' +
                            displayData[ii][slportIdx] + '</td></tr>';
                mirhtml += '<tr><td></td><td class="table-text-1">'+ displayData[ii][typeIdx] +'</td>';
                mirhtml += '<td class="table-text-1">'+ displayData[ii][statusIdx] +'</td>';
                mirhtml += '<td class="table-text-1">'+ displayData[ii][txidIdx] +'</td>';
                /* insert button container */
                mirhtml += '<td class="table-text-1">';

                /* DB name is @ master host:port */
                btnargs[0] = displayData[ii][dbnameIdx]+'@'+
                                req.tab.connection.host+':'+req.tab.connection.port;
                btnargs[1] = displayData[ii][slhostIdx];//specify slave's host
                btnargs[2] = displayData[ii][slportIdx];
                mirhtml += generateButton('Stop', 'StopM'+ii, null,
                                 'stopSlaveClick', btnargs);
                mirhtml += '</td>';
                mirhtml += '</tr>';
            }
        }
        else
        {
            // No results, display an empty row
            mirhtml += '<tr><td class="table-text-1" colspan="5">None</td></tr>';
        }

        mirhtml += '</table>';
        mirhtml += '</center>';

        box = getBox('DataflowContainer');
        box.updateChild('MasterFlowContainer', mirhtml);
    }
}

/* =======================================================================
*/
function ajaxSlaveFlowListHandler(req)
{
    var response;
    var mirhtml;
    var numslaves;
    var mywidth;
    var container;
    var box;
    var btnargs = new Array();
    var dbnameIdx = 0; // Index of DbName in rdmNames
    var slhostIdx = 1;
    var slportIdx = 2;
    var typeIdx = 3;   // Index of Type in rdmNames
    var statusIdx = 4; // Index of Status in rdmNames
    var syncIdx = 5;   // Index of Sync in rdmNames
    var txidIdx = 6;   // Index of last txid in rdmNames
    var odbcIdx = 7;   // Index of ODBC in rdmNames
    var displayData = new Array();
    /* Names returned by dbmirror */
    var rdmNames = new Array(
        'DbName',
        'SlaveHost',
        'SlavePort',
        'Type',
        'Status',
        'Sync',
        'LastTxid',
        'ODBC'
    );
    var status_str = new Array (
        'Starting',
        'Not-Ready',
        'Ready',
        'Not-Running'
    );

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 1, req.tab.connection)) == '')
            return;

        numslaves = parseResponse(response, displayData, rdmNames[0]);

        for (var ii = 0; ii < numslaves; ++ii)
        {
            /* status is returned as a number, convert to a string */
            displayData[ii][statusIdx] = status_str[displayData[ii][statusIdx]];
            displayData[ii][typeIdx] = getMirServerType(displayData[ii][typeIdx],
                                            displayData[ii][syncIdx],
                                            displayData[ii][odbcIdx]);
        }

        container = document.getElementById('DataflowContainer');
        mywidth = container.offsetWidth-BOUNDARY_BOX_INDENT;

        mirhtml = '<br><center><table border="0" width="'+mywidth+'">';

        //Title row
        mirhtml += '<tr><td class="box-subtitle-text-with-bkgrnd">Slave of</td><td class="box-subtitle-text-with-bkgrnd">Type</td><td class="box-subtitle-text-with-bkgrnd">Status</td><td class="box-subtitle-text-with-bkgrnd">Txid</td><td class="box-subtitle-text-with-bkgrnd">Action</td></tr>';

        if (numslaves > 0)
        {
            for (var ii = 0; ii < displayData.length; ++ii)
            {
                if (displayData[ii][0] == "")
                    continue;

                //Data row(s)
                mirhtml += '<tr>';
                mirhtml += '<td class="table-text-2" colspan="5">'+
                            displayData[ii][dbnameIdx] + '@' +
                            displayData[ii][slhostIdx] + ':' +
                            displayData[ii][slportIdx] + '</td></tr>';
                mirhtml += '<tr><td></td><td class="table-text-1">'+ displayData[ii][typeIdx] +'</td>';
                mirhtml += '<td class="table-text-1">'+ displayData[ii][statusIdx] +'</td>';
                mirhtml += '<td class="table-text-1">'+ displayData[ii][txidIdx] +'</td>';
                /* insert button container */
                mirhtml += '<td class="table-text-1">';

                /* DB name is @ master host:port */
                btnargs[0] = displayData[ii][dbnameIdx] +'@'+
                                displayData[ii][slhostIdx] +':'+
                                displayData[ii][slportIdx];
                btnargs[1] = req.tab.connection.host; //This tab is the slave
                btnargs[2] = req.tab.connection.port; //Use its connection
                mirhtml += generateButton('Stop', 'StopS'+ii, null,
                                 'stopSlaveClick', btnargs);
                mirhtml += '</td>';
                mirhtml += '</tr>';
            }
        }
        else
        {
            // No results, display an empty row
            mirhtml += '<tr><td class="table-text-1" colspan="5">None</td></tr>';
        }

        mirhtml += '</table>';
        mirhtml += '</center>';

        box = getBox('DataflowContainer');
        box.updateChild('SlaveFlowContainer', mirhtml);
    }
}

/* =======================================================================
*/
function ajaxFlowStartHandler(req)
{
    var response;
    var displayData = new Array();
    /* Names returned by tfs */
    var rdmNames = new Array(
        'DbName',
        'Result'
    );

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 1, req.tab.connection)) == '')
            return;

        parseResponse(response, displayData, rdmNames[0]);

        if (displayData[0][1] != '0')
            alert(getErrorString(mapRdmError(displayData[0][1])));

        gButtons[req.userdata].enableButton();

        /* Reget db names as we may be starting a slave for the first time */
        getDbNames();
    }
}

/* =======================================================================
*/
function ajaxFlowStopHandler(req)
{
    var response;
    var displayData = new Array();
    /* Names returned by tfs */
    var rdmNames = new Array(
        'DbName',
        'Result'
    );

    if (req.readyState == 4 /* complete */ && req.status == 200)
    {
        if ((response=validateLogin(req.responseText, 1, req.tab.connection)) == '')
            return;

        parseResponse(response, displayData, rdmNames[0]);

        if (displayData[0][1] != '0')
            alert(getErrorString(mapRdmError(displayData[0][0])));

        removeButtonById(req.userdata);
        resumeRefresh();

        if (getRefreshRate() == 0)
            setTimeout(reloadPage, 2000);
    }
}


/* =======================================================================
    Fills DatabaseContainer with links for pages representing each database.
*/
function displayDatabaseNames()
{
    var db;
    var html;
    var mywidth;
    var box;
    var tab = getSelectedTab();

    db = document.getElementById('DatabaseContainer');
    mywidth = db.offsetWidth-BOUNDARY_BOX_INDENT;

    html = '<table border="0" width="'+mywidth+'">';

    for (var ii = 0; ii < tab.connection.dbobjs.length; ++ii)
    {
        html += '<tr><td width="'+BOUNDARY_BOX_INDENT+'"></td><td><a onClick="dbNameClick('+ii+');return false;" href="javascript:void(0);" class="box-link-text">'+tab.connection.dbobjs[ii].dbname+'</a>';
        html += '</td></tr>';
    }

    html += '</table></center>';
    box = getBox('DatabaseContainer');
    box.update(html);
}

/* =======================================================================
*/
function displayDataflowIdentity()
{
    var identity = '';
    var box;
    var tab = getSelectedTab();

    if (!tab || !tab.connection)
        return;

    if (tab.connection.identity == '')
    {
        identity += 'Dataflow';
        box = getBox('DataflowContainer');
        box.updateChild('MasterFlowContainer', '<table border="0"><tr><td width="50">&nbsp;</td><td class="table-text-1">No mirroring or replication server found</td></tr></table>');
        box.updateChild('SlaveFlowContainer', '');
        box.updateChild('StartFlowContainer', '');
        if (tab.connection.dbobjs)
            setTimeout(getIdentity, 2000);
    }
    else if (tab.connection.identity == 'RDM_DBREPSQL')
        identity += 'Dataflow - SQL Replication Server';
    else if (tab.connection.identity == 'RDM_DBREP')
        identity += 'Dataflow - Replication Server';
    else if (tab.connection.identity == 'RDM_DBMIRROR')
        identity += 'Dataflow - Mirroring Server';
    else
        identity += 'Dataflow - Loading...';

    box = getBox('DataflowContainer');
    box.setTitle(identity);
}

/* =======================================================================
    Fill in the database name list box when a new master is selected
    in the 'start slave' section.
*/
function fillStartFlowDbList()
{
    var selectedMaster = document.getElementById('StartFlowSelectMaster');
    var selectedDbs = document.getElementById('StartFlowSelectDbs');
    var selected;
    var ii;

    selected = selectedMaster.options[selectedMaster.selectedIndex].value;

    selectedDbs.options.length = 0;

    for (ii = 0; ii < gTabObjs[selected].connection.dbobjs.length; ++ii)
    {
        selectedDbs.options[ii] = new Option(gTabObjs[selected].connection.dbobjs[ii].dbname, gTabObjs[selected].connection.dbobjs[ii].dbname);
    }
}

/* =======================================================================
*/
function displayDataflowStart()
{
    var container;
    var mywidth;
    var mirhtml;
    var ii;
    var box;
    var tab = getSelectedTab();
    var btnargs = new Array();
    var tfstabcount = 0;

    box = getBox('DataflowContainer');

    if (!tab || !tab.connection || gTabObjs.length <= 1 ||
         tab.connection.identity == '')
    {
        box.updateChild('StartFlowContainer', '');
        return;
    }

    for (ii = 0; ii < gTabObjs.length; ++ii)
    {
        if (gTabObjs[ii].tabtype != PAGETYPE_TFS ||
                (gTabObjs[ii].connection.host == tab.connection.host &&
                 gTabObjs[ii].connection.port == tab.connection.port))
            continue;
        tfstabcount++;
    }
    if (tfstabcount <= 0)
    {
        box.updateChild('StartFlowContainer', '');
        return;
    }

    container = document.getElementById('DataflowContainer');
    mywidth = container.offsetWidth-BOUNDARY_BOX_INDENT;

    mirhtml = '<br><center><table border="0" width="'+mywidth+'">';

    //Title row
    mirhtml += '<tr><td colspan="4" class="box-subtitle-text-with-bkgrnd">Start slave on '+tab.connection.host+':'+tab.connection.port+' from</td></tr>';
    mirhtml += '<tr><td width="'+BOUNDARY_BOX_INDENT+'" class="table-text-1"></td><td class="table-text-1">Connected Masters</td><td class="table-text-1">Databases</td><td></td></tr>';

    mirhtml += '<tr><td width="'+BOUNDARY_BOX_INDENT+'" class="table-text-1"></td><td><select id="StartFlowSelectMaster" class="table-text-1" onchange="fillStartFlowDbList();">';
    for (ii = 0; ii < gTabObjs.length; ++ii)
    {
        if (gTabObjs[ii].tabtype != PAGETYPE_TFS ||
                (gTabObjs[ii].connection.host == tab.connection.host &&
                 gTabObjs[ii].connection.port == tab.connection.port))
            continue;
        mirhtml += '<option value="' + ii + '">' + gTabObjs[ii].getText();
    }

    mirhtml += '</select></td><td><select id="StartFlowSelectDbs" class="table-text-1">';
    mirhtml += '</select></td>';

    mirhtml += '<td>' + generateButton('Start', 'StartSlave', null,
                         'startSlaveClick', btnargs) + '</td></tr>';

    mirhtml += '<tr><td width="'+BOUNDARY_BOX_INDENT+'"></td><td colspan="3" class="box-text"><input type="checkbox" id="StartFlowOverrideInMem">Override In-Memory';
    if (tab.connection.identity == 'RDM_DBMIRROR')
        mirhtml += '<input type="checkbox" id="StartFlowSynchronous">Synchronous';
    mirhtml += '</td></tr>';
    if (tab.connection.identity == 'RDM_DBREPSQL')
    {
        mirhtml += '<tr><td width="'+BOUNDARY_BOX_INDENT+'"></td><td class="box-text"><form id="StartFlowSQLGroup"><input type="radio" name="sqlgroup" value="RDMs" checked>RDMs<input type="radio" name="sqlgroup" value="MySQL">MySQL<br><input type="radio" name="sqlgroup" value="MSSQL">MSSQL<input type="radio" name="sqlgroup" value="Oracle">Oracle</form></td>';
        mirhtml += '<td colspan="2" class="box-text" valign="top">DSN<br><input type="text" id="StartFlowSQLDSN" class="box-text" size="30"></td></tr>';
    }

    mirhtml += '</table>';
    mirhtml += '</center>';

    box.updateChild('StartFlowContainer', mirhtml);

    fillStartFlowDbList();
}

/* =======================================================================
*/
function displayDataflowSettings()
{
    var container;
    var mywidth;
    var mirhtml;
    var box;
    var tab = getSelectedTab();
    var actionstr;
    var actionPrefix = 'action=getini&';
    var idPrefix = '';
    var containerName = 'DataflowSettingsContainer';
    var btnarg = new Array();
    var tabid = getCookie('tabpage-databases');
    var dbname = '';

    if (tab.tabtype == PAGETYPE_DATABASE)
    {
        dbname = tab.connection.dbobjs[tabid].dbname;

        containerName = 'DbDataflowSettingsContainer';
        actionPrefix += 'dbname=' + dbname + '&';
        idPrefix = 'db';
    }

    container = document.getElementById(containerName);
    mywidth = container.offsetWidth-BOUNDARY_BOX_INDENT;

    mirhtml = '<center><table border="0" width="'+mywidth+'">';

    mirhtml += '<tr><td class="table-text-1">Create mirroring log files:</td><td class="table-text-1"><input type="checkbox" id="'+idPrefix+'SettingsMirLog"></td><td></td></tr>';

    mirhtml += '<tr><td class="table-text-2">Create replication log files:</td><td class="table-text-2"><input type="checkbox" id="'+idPrefix+'SettingsRepLog"></td><td></td></tr>';

    mirhtml += '<tr><td class="table-text-1">Single slave mode:</td><td class="table-text-1"><input type="checkbox" id="'+idPrefix+'SettingsSingleSlave" checked></td><td></td></tr>';

    mirhtml += '<tr><td class="table-text-2">Keep log files younger than:</td><td class="table-text-2"><input type="text" id="'+idPrefix+'FlowLogAge" class="table-text-2" size="2"><select id="'+idPrefix+'SettingsMirLogAge" class="table-text-2">';
    mirhtml += '<option value="s">secs';
    mirhtml += '<option value="m">mins';
    mirhtml += '<option value="h">hrs';
    mirhtml += '<option value="d">days';
    mirhtml += '</select></td><td></td></tr>';

    mirhtml += '<tr><td class="table-text-1">Keep log files until used space exceeds:</td><td class="table-text-1"><input type="text" id="'+idPrefix+'FlowLogSpace" class="table-text-1" size="2"><select id="'+idPrefix+'SettingsMirLogSpace" class="table-text-1">';
    mirhtml += '<option value="k">Kb';
    mirhtml += '<option value="m">Mb';
    mirhtml += '<option value="g">Gb';
    mirhtml += '</select></td><td></td></tr>';

    btnarg[0] = dbname;
    mirhtml += '<tr><td></td><td colspan="2"><table border="0"><tr><td>' +
                    generateButton('Apply', idPrefix+'ApplySettings', null,
                                   'applySettingsClick', btnarg);
    if (dbname != '')
        mirhtml += '</td><td>'+generateButton('Default', idPrefix+'RevertSettings', null,
                                    'revertSettingsClick', btnarg);
    mirhtml += '</td></tr></table></td></tr>';
    mirhtml += '</table>';
    mirhtml += '</center>';

    box = getBox(containerName);
    if (dbname != '')
        box.setTitle('Dataflow Settings on this database');// + tab.getText());
    box.updateChild(idPrefix+'DataflowSettings', mirhtml);

    /* request ini settings */
    actionstr = actionPrefix + "section=configuration&key=mirroring&";
    requestAction(actionstr, ajaxINIGetHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, dbname);

    actionstr = actionPrefix + "section=configuration&key=replication&";
    requestAction(actionstr, ajaxINIGetHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, dbname);

    actionstr = actionPrefix + "section=configuration&key=SingleSlaveMode&";
    requestAction(actionstr, ajaxINIGetHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, dbname);

    actionstr = actionPrefix + "section=LogCleanup&key=LogFileAge&";
    requestAction(actionstr, ajaxINIGetHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, dbname);

    actionstr = actionPrefix + "section=LogCleanup&key=LogFileSpace&";
    requestAction(actionstr, ajaxINIGetHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, dbname);

}

/* =======================================================================
    Return the HTML for a field's data type
*/
function displayFieldType(field)
{
    var fdtype = field.getFieldType();
    var flags = field.getFieldFlags();
    var typestr = '';

    if (flags & 0x04)
    {
        typestr = (fdtype == 's' || fdtype == 'i' || fdtype == 'l' ||
                        fdtype == 'L') ? 'u' : 'unsigned ';
    }

    switch (fdtype)
    {
        case 'f':   typestr += 'float'; break;
        case 'F':   typestr += 'double'; break;
        case 'c':   typestr += 'char'; break;
        case 'C':   typestr += 'wchar_t'; break;
        case 'v':   typestr += 'varchar'; break;
        case 'V':   typestr += 'wvarchar'; break;
        case 'n':   typestr += 'varbinary'; break;
        case 's':   typestr += 'int16_t'; break;
        case 'i':   typestr += 'int32_t'; break;
        case 'l':   typestr += 'int32_t'; break;
        case 'L':   typestr += 'int64_t'; break;
        case 'D':   typestr += 'dbaddr'; break;
        case 'b':   typestr += 'blob'; break;
        case 'd':   typestr += 'dbaddr4'; break;
        case 'a':   typestr += 'dbaddr8'; break;
//         case 'g':   typestr += 'struct'; break;
//         case 'k':   typestr += 'compound key'; break;
    }

    return typestr;
}

/* =======================================================================
    Return the HTML for a field's dimension.
*/
function displayFieldDims(field)
{
    var dim0 = field.getFieldDim0();
    var dim1 = field.getFieldDim1();
    var dim2 = field.getFieldDim2();
    var dimstr = '';

    if (dim0 != '0')
        dimstr += '[' + dim0 + ']';
    if (dim1 != '0')
        dimstr += '[' + dim1 + ']';
    if (dim2 != '0')
        dimstr += '[' + dim2 + ']';

    return dimstr;
}

/* =======================================================================
*/
function displayFieldKey(field)
{
    var fdkey = field.getFieldKey();
    var fdtype = field.getFieldType();
    var keystr = '';

    if (fdtype == 'k')
        keystr += 'compound ';
        
    switch (fdkey)
    {
        case 'd':   keystr += 'key '; break;
        case 'u':   keystr += 'unique key '; break;
        case 'h':   keystr += 'hash '; break;
    }

    return keystr;
}

/* =======================================================================
*/
function displayKeySort(key)
{
    var html = '';
    var keysort = key.getKeySort();
    
    switch (keysort)
    {
        case 'a':   html += 'ascending'; break;
        case 'd':   html += 'descending'; break;
    }
    
    return html;
}

/* =======================================================================
    Display the field rows of a record.
*/
function displayRecordFields(record, fields)
{
    var html = '';
    var jj;
    var len;
    var field;
    var fdtype;
    var fdflags;
    var structfld;
    var first_field;
    
    len = record.getNumFields();
    if (len == 65535)
        return html;
            
    first_field = record.getField();
    
    for (jj = first_field, structfld = ''; jj < first_field + len; ++jj)
    {
        field = fields[jj];
        fdtype = field.getFieldType();
        fdflags = field.getFieldFlags();

        if (fdtype == 'k')
            continue;  // display key fields later
            
        html += '<tr><td width="10">&nbsp;</td><td>';

        if (fdflags & 0x02)
        {
            // indent fields within a struct
            html += '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
        }

        if (fdtype == 'g') // begin struct/group type
        {
            structfld = '<b>' + field.getFieldName() + '</b>' +
                                displayFieldDims(field);
            html += displayFieldKey(field) + 'struct {</td></tr>';
            continue;
        }
        else if (structfld != '' && !(fdflags & 0x02))
        {
            // end of struct
            html += '} ' + structfld + '</td></tr>';
            structfld = '';
            html += '<tr><td width="10">&nbsp;</td><td>';
        }

        html += displayFieldKey(field) + displayFieldType(field);
        html += '</td><td width="10">&nbsp;</td><td>' +
                              '<b>' + field.getFieldName() + '</b>' +
                              displayFieldDims(field);
        html += '</td></tr>';
    }

    if (structfld != '')
    {
        // end of struct
        html += '<tr><td width="10">&nbsp;</td><td>';
        html += '} ' + structfld + '</td></tr>';
    }
    
    return html;
}

/* =======================================================================
    Display all compound keys for a record.
*/
function displayCompoundKeys(record, fields)
{
    var html = '';
    var ii;
    var compkey = record.getRecordFlags() & 0x10 ? 1 : 0;
    var key;
    var last_kid = -1;
    var field;
    var cfield;
    var len = record.getNumKeyFields();

    if (compkey)
    {
        for (ii = 0; ii < len; ++ii)
        {
            key = record.getKeyField(ii);
            field = fields[key.getKeyField()];
            cfield = fields[key.getCompKey()];

            if (key.getKeyField() != last_kid)
            {
                html += '<tr><td width="10">&nbsp;</td><td>';
                html += displayFieldKey(field);
                html += '</td><td width="10">&nbsp;</td><td>' +
                                      '<b>' + field.getFieldName() + 
                                      '</b> {</td></tr>';
            }

            // display fields that make up the compound key
            html += '<tr><td width="10">&nbsp;</td><td>&nbsp;';
            html += '</td><td width="10">&nbsp;</td><td>' +
                                  '<b>' + cfield.getFieldName() + 
                                  '</b> ' + displayKeySort(key) +
                                  '</td></tr>';
            
            last_kid = key.getKeyField();
            
            if (ii+1 == len ||
                (ii+1 < len && record.getKeyField(ii+1).getKeyField() != last_kid))
                html += '<tr><td width="10">&nbsp;</td><td>}</td></tr>';
        }
    }
    
    return html;
}

/* =======================================================================
*/
function displaySortOrder(set)
{
    var html = '';
    var order= set.getOrder();
    
    switch (order)
    {
        case 'a':   html += 'ascending'; break;
        case 'd':   html += 'descending'; break;
        case 'f':   html += 'first'; break;
        case 'l':   html += 'last'; break;
    }
    
    return html;
}


/* =======================================================================
    Display the contents of a set (owner, members, sort).
*/
function displaySetInfo(records, fields, set)
{
    var html = '';
    var ii, jj;
    var member;
    var numMembers = set.getNumMembers();
    var numSortFields;
    
    html += '<tr><td width="10">&nbsp;</td><td>order <td width="10">&nbsp;</td><td><b>';
    html += displaySortOrder(set);
    html += '</b></td></tr>';

    html += '<tr><td width="10">&nbsp;</td><td>owner <td width="10">&nbsp;</td><td><b>';
    html += records[set.getOwner()].getRecordName();
    html += '</b></td></tr>';

    for (ii = 0; ii < numMembers; ++ii)
    {
        member = set.getMember(ii);
        html += '<tr><td width="10">&nbsp;</td><td>member <td width="10">&nbsp;</td><td><b>';
        html += records[member.getRecord()].getRecordName();
        html += '</b>';
        
        if ((numSortFields = member.getNumSortFields()) > 0)
        {
            html += ' by ';
            for (jj = 0; jj < numSortFields; ++jj)
            {
                html += '<b>'+fields[member.getSortField(jj).getField()].getFieldName()+'</b>';
                if (jj + 1 < numSortFields)
                    html += ', ';
            }
        }
        
        html += '</td></tr>';
    }
    
    return html;
}

/* =======================================================================
    Display the properties of a file (slots, slsize, pgsize, record names).
*/
function displayFileInfo(records, fields, file, fileidx)
{
    var html = '';
    var ii, jj;
    var numRecords = records.length;
    var numFields = file.getNumFields();
    var field;
    var fieldrec;
    var comma = '';
    
    html += '<tr><td width="10">&nbsp;</td><td>Page Size: <td width="10">&nbsp;</td><td><b>';
    html += file.getPageSize();
    html += '</b></td></tr>';

    html += '<tr><td width="10">&nbsp;</td><td>Slots Per Page: <td width="10">&nbsp;</td><td><b>';
    html += file.getSlots();
    html += '</b></td></tr>';

    html += '<tr><td width="10">&nbsp;</td><td>Slots Size: <td width="10">&nbsp;</td><td><b>';
    html += file.getSlotSize();
    html += '</b></td></tr>';

    html += '<tr><td width="10">&nbsp;</td><td>Contents: <td width="10">&nbsp;</td><td>';

    for (ii = 0; ii < numRecords; ++ii)
    {
        if (records[ii].getFile() == fileidx)
        {
            html += comma + '<b>' + records[ii].getRecordName() + '</b>';
            comma = ', ';
        }
    }
    
    for (ii = 0; ii < numFields; ++ii)
    {
        field = fields[file.getField(ii)];
        fieldrec = field.getFieldRecno();
        
        html += comma + '<b>' + records[fieldrec].getRecordName() + '.' + 
                            field.getFieldName() + '</b>';
        comma = ', ';
    }
    
    html += '</td></tr>';
    
    return html;
}

/* =======================================================================
*/
function displayFileType(file)
{
    var html = '';
    var filetype= file.getFileType();
    
    switch (filetype)
    {
        case 'd':   html += 'data'; break;
        case 'k':   html += 'key'; break;
        case 'v':   html += 'vardata'; break;
        case 'h':   html += 'hash'; break;
        case 'b':   html += 'blob'; break;
    }
    
    return html;
}

/* =======================================================================
*/
function displaySchemaContainer()
{
    var ii;
    var container;
    var html;
    var mywidth;
    var box;
    var tab = getSelectedTab();
    var tabid = getCookie('tabpage-databases');
    var rt;
    var records;
    var fields;
    var sets;
    var files;

    records = tab.connection.dbobjs[tabid].records;
    fields = tab.connection.dbobjs[tabid].fields;
    sets = tab.connection.dbobjs[tabid].sets;
    files = tab.connection.dbobjs[tabid].files;

    container = document.getElementById('SchemaContainer');
    mywidth = container.offsetWidth-BOUNDARY_BOX_INDENT;

    html = '<center><table border="0" width="'+mywidth+'">';

    if (records.length > 0)
    {
        for (ii = 0; ii < records.length; ++ii)
        {
            html += '<tr><td class="table-text-'+(ii%2+1)+'">';

            html += 'record <b>' + records[ii].getRecordName() + '</b> (<table border="0">';
            html += displayRecordFields(records[ii], fields);
            html += displayCompoundKeys(records[ii], fields);
            
            html += '</table>)</td></tr>';
        }
        
        if (sets.length > 0)
            html += '<tr><td><hr></td></tr>';
        
        for (ii = 0; ii < sets.length; ++ii)
        {
            html += '<tr><td class="table-text-'+(ii%2+1)+'">';

            html += 'set <b>' + sets[ii].getSetName() + '</b> (<table border="0">';
            html += displaySetInfo(records, fields, sets[ii]);
            
            html += '</table>)</td></tr>';
        }
        
        html += '<tr><td><hr></td></tr>';
        
        for (ii = 0; ii < files.length; ++ii)
        {
            html += '<tr><td class="table-text-'+(ii%2+1)+'">';

            html += displayFileType(files[ii]) + ' file <b>' + files[ii].getFileName() + '</b> (<table border="0">';
            html += displayFileInfo(records, fields, files[ii], ii);
            
            html += '</table>)</td></tr>';
        }
    }
    else
        html += '<td class="table-text-1">Schema data not found</td></tr>';
    html += '</table></center>';

    box = getBox('SchemaContainer');
    box.update(html);
}

/* =======================================================================
    Change the html for the main page for a server.
*/
function displayServerPage()
{
    var tab = getSelectedTab();

    setPageType(PAGETYPE_TFS);

    if (tab.connection && tab.connection.dbobjs == undefined)
        getDbNames();

    displayDatabaseNames();
    displayDataflowIdentity();
    displayDataflowStart();
    displayDataflowSettings();
}

/* =======================================================================
*/
function displayDatabasePage()
{
    var tab = getSelectedTab();
    var tabid = getCookie('tabpage-databases');

    setPageType(PAGETYPE_DATABASE);
    displayDataflowSettings();
}

/* =======================================================================
*/
function displayLoginPage()
{
    var html;
    var button;
    var logintable;
    var buttonHTML;

    pauseRefresh();
    setPageType(PAGETYPE_LOGIN);

    document.getElementById('refreshDiv').style.visibility = 'hidden';
    document.getElementById('refresh0Div').style.visibility = 'hidden';
    document.getElementById('refresh1Div').style.visibility = 'hidden';
    document.getElementById('refresh2Div').style.visibility = 'hidden';

    buttonHTML = generateButton('Login', 'Login', null, 'loginClick', null);

    html = '<center><br><br><table id="logintable" border=0><tr><td valign="top">';

    html += '<center><table border="0" class="borderless-table" width="225">';

    html += '<tr><td class="table-text-1" width="60" valign="middle">&nbsp;Username</td><td class="table-text-1" valign="middle"><input type="text" id="username" name="username" size="25" class="table-text-1"></td></tr>';
    html += '<tr><td class="table-text-1" valign="middle">&nbsp;Password</td><td class="table-text-1" valign="middle"><input type="password" id="password" name="password" size="25" class="table-text-1" onKeyDown="if (event.keyCode == 13) loginClick()"></td></tr>';
    html += '</table>';
    html += '<tr><td>' + buttonHTML + '</td></tr>';
    html += '</table></center><p>';


    /* Because IE still doesn't work like other browsers, must do
       focus and html set in a timer.
     */
    setTimeout(function() { document.getElementById('LoginContainer').innerHTML = html; }, 5);
    setTimeout(function() { document.getElementById('username').focus(); }, 200);
}

/* =======================================================================
    Display page used to connect to different server
*/
function displayNewPage()
{
    var buttonHTML = generateButton('Connect', 'Connect', null, 'connectClick', null);
    var html;
    var btnarg = new Array();
    var helpbtn;

    btnarg[0] = 'HelpNewConnection';
    helpbtn = generateButton('Help', 'HelpNewConnection', 'question.png',
                                      'helpClick', btnarg);
    setButtonHelpText('HelpNewConnection', H_NEWCONNECTION);

    pauseRefresh();
    setPageType(PAGETYPE_CONNECT);

    html = '<center><br><br><table id="connecttable" border="0"><tr><td valign="top">';

    html += '<center><table border="0" class="borderless-table" width="225">';

    html += '<tr><td class="table-text-1" width="40" valign="middle">&nbsp;Host</td><td class="table-text-1" valign="middle"><input type="text" id="connect_hostname" name="connect_hostname" size="25" class="table-text-1"></td><td>'+helpbtn+'</td></tr>';
    html += '<tr><td class="table-text-1" valign="middle">&nbsp;Port</td><td class="table-text-1" valign="middle"><input type="text" id="connect_port" name="connect_port" size="25" class="table-text-1" onKeyDown="if (event.keyCode == 13) connectClick(\'Connect\')"></td><td></td></tr>';
    html += '</table>';
    html += '<tr><td>' + buttonHTML + '</td></tr>';
    html += '</table></center><p>';

    /* Because IE still doesn't work like other browsers, must do
       focus and html set in a timer.
     */
    setTimeout(function() { document.getElementById('ConnectContainer').innerHTML = html; }, 5);
    setTimeout(function() { document.getElementById('connect_hostname').focus(); }, 200);
}

/* =======================================================================
    Called by the pageObj.boxes.refresh call
*/
function refreshDbLocksContainer()
{
    var tabid = getCookie('tabpage-databases');
    var actionstr;
    var tab = getSelectedTab();

    actionstr = "action=dblocks&dbname="+tab.connection.dbobjs[tabid].dbname;
    requestAction(actionstr, ajaxDatabasesLocksHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, null);
}

/* =======================================================================
    Called by the pageObj.boxes.refresh call
*/
function refreshDbUsersContainer()
{
    var tabid = getCookie('tabpage-databases');
    var actionstr;
    var tab = getSelectedTab();

    actionstr = "action=dbusers&dbname="+tab.connection.dbobjs[tabid].dbname;
    requestAction(actionstr, ajaxDatabasesUsersHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, null);
}

/* =======================================================================
    Build a database info page.
*/
function refreshDatabasePage()
{
    gPageObjs[PAGETYPE_DATABASE].refresh();
}

/* =======================================================================
    Called by the pageObj.boxes.refresh call
*/
function refreshUsersContainer()
{
    var actionstr;
    var tab = getSelectedTab();

    actionstr = "action=userlist";
    requestAction(actionstr, ajaxUsersResponseHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port), null, null);
}

/* =======================================================================
    Called by the pageObj.boxes.refresh call
*/
function refreshDataflowContainer()
{
    var actionstr;
    var tab = getSelectedTab();

    actionstr = "action=flowlist&type=master&";
    requestAction(actionstr, ajaxMasterFlowListHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port) + MIRROR_PORT_OFFSET, null, null);

    actionstr = "action=flowlist&type=slave&";
    requestAction(actionstr, ajaxSlaveFlowListHandler, 'proxy.rdm', tab.connection.host,
                    parseInt(tab.connection.port) + MIRROR_PORT_OFFSET, null, null);

}

/* =======================================================================
    Server page displays databases, users and dataflow information.
*/
function refreshServerPage()
{
    var actionstr;
    var tab = getSelectedTab();

    if (tab.connection != undefined && tab.connection.identity == '')
        displayDataflowIdentity();

    gPageObjs[PAGETYPE_TFS].refresh();
}

/* =======================================================================
    Since schema xml files can be large and won't change while in use,
    cache the schema once it's downloaded in tab.connection.dbobjs[].records.
*/
function refreshSchemaContainer()
{
    var actionstr;
    var tab = getSelectedTab();
    var tabid = getCookie('tabpage-databases');
    var records = tab.connection.dbobjs[tabid].records;
    var container;
    var mywidth;
    var html;
    var box;

    if (records.length == 0)
    {
        container = document.getElementById('SchemaContainer');
        mywidth = container.offsetWidth-BOUNDARY_BOX_INDENT;
    
        html = '<center><table border="0" width="'+mywidth+'">';
        html += '<tr><td class="table-text-1">Loading...</td></tr>';
        html += '</table></center>';
        box = getBox('SchemaContainer');
        box.update(html);

        actionstr = "action=getschema&dbname="+tab.connection.dbobjs[tabid].dbname;
        requestAction(actionstr, ajaxSchemaResponseHandler, 'proxy.rdm',
                    tab.connection.host, parseInt(tab.connection.port), null, null);
    }
    else
        displaySchemaContainer();
}


/* =======================================================================
    Only refresh the sections that need refreshing on a selected page.
*/
function refreshContent()
{
    if (gUsername == '' || gEnc == null)
    {
        /* prompt for login information */
        displayLoginPage();
        return;
    }

    keepTabsAlive();

    switch (getCurrentTabType())
    {
        case PAGETYPE_TFS:
            refreshServerPage();
            return 0;
        case PAGETYPE_DATABASE:
            refreshDatabasePage();
            return 0;
        default:
            alert("Bad page type: " + getCurrentTabType());
            break;
    }
}

/* =======================================================================
    Displays the whole page
*/
function DisplayContent()
{
    if (gPagesInitialized == 0)
        initPages();

    if (gUsername == '' || gEnc == null)
    {
        /* prompt for login information */
        displayLoginPage();
        return;
    }

    /* Display the 'logout' and 'refresh' links */
    document.getElementById('logoutDiv').style.visibility = 'visible';
    document.getElementById('refreshDiv').style.visibility = 'visible';
    document.getElementById('refresh0Div').style.visibility = 'visible';
    document.getElementById('refresh1Div').style.visibility = 'visible';
    document.getElementById('refresh2Div').style.visibility = 'visible';

    switch (getCurrentTabType())
    {
        case PAGETYPE_TFS:
            displayServerPage();
            break;
        case PAGETYPE_CONNECT:
            displayNewPage();
            return 0;
        case PAGETYPE_DATABASE:
            displayDatabasePage();
            break;
        default:
            alert("Bad page type: " + getCurrentTabType());
            break;
    }

    refreshContent();

    return 0;
}

