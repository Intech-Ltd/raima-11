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

/********************** Customization section ****************************/
var copyrightString = '&copy; Copyright 2011 Raima, Inc. All rights reserved.';

/* Configure the three refresh rates available to the user (in seconds)
   First entry should always be 0.
 */
var refreshRates = new Array(0, 2, 5);

/* Configure the width of the display by changing MAIN_TBL_WIDTH */
var MAIN_TBL_WIDTH              = 640;

var MAIN_TBL_MARGIN             = 20;
var PAGE_MARGIN                 = 30;


/************************** Globals *************************************/
var refreshTimer;
var refreshIndex = 1;


/* =======================================================================
*/
function init()
{
    refreshIndex = getCookie('refreshrate');
    if (refreshIndex == '' || refreshIndex > 2 || refreshIndex < 0) 
    {
        refreshIndex = 1;
        setCookie("refreshrate", refreshIndex);
    }
    
    if (refreshRates[refreshIndex] > 0)
        refreshTimer = setTimeout(refreshPage, refreshRates[refreshIndex] * 1000);
}

/* =======================================================================
*/
function setCookie(name, value, expiredays)
{
    var exdate = new Date();
    
    exdate.setDate(exdate.getDate() + expiredays);
    
    document.cookie = name + "=" + escape(value) +
    ((expiredays==null) ? "" : ";expires="+exdate.toGMTString());
}

/* =======================================================================
*/
function getCookie(name)
{
    var start;
    var end;
    
    if (document.cookie.length > 0)
    {
        start = document.cookie.indexOf(name + "=");
        if (start != -1)
        {
            start = start + name.length + 1;
            end = document.cookie.indexOf(";", start);
            if (end == -1)
                end = document.cookie.length;
            return unescape(document.cookie.substring(start, end));
        }
    }
    return "";
}

/* =======================================================================
*/
function getMainTableWidth()
{
    return MAIN_TBL_WIDTH;
}

/* =======================================================================
*/
function getMainTableMargin()
{
    return MAIN_TBL_MARGIN;
}

/* =======================================================================
*/
function getPageMargin()
{
    return PAGE_MARGIN;
}

/* =======================================================================
*/
function reloadPage()
{
    DisplayContent();
}

/* =======================================================================
*/
function refreshPage()
{
    refreshContent();
    refreshTimer = setTimeout(refreshPage, refreshRates[refreshIndex] * 1000);
}

/* =======================================================================
    Set the refresh rate.  'rate' is an index into the refreshRates array.
*/
function setRefreshRate(rate)
{
    document.getElementById("refresh"+refreshIndex+"A").className="settings";
    
    refreshIndex = rate;
    setCookie("refreshrate", rate);
    resumeRefresh();

    document.getElementById("refresh"+refreshIndex+"A").className="settings_on";
}

/* =======================================================================
    Returns the currently active refresh rate index.
*/
function getRefreshRate()
{
    return refreshIndex;
}
/* =======================================================================
    Temporarily stop the refresh
*/
function pauseRefresh()
{
    clearTimeout(refreshTimer);
}

/* =======================================================================
    Restart the paused refresh
*/
function resumeRefresh()
{
    clearTimeout(refreshTimer);
    if (refreshRates[refreshIndex] > 0)
        refreshTimer = setTimeout(refreshPage, refreshRates[refreshIndex] * 1000);
}

/* =======================================================================
*/
function getCopyright()
{
    var copyright;
    
    copyright = '<table border=0 cellspacing=0 cellpadding=0 width='+MAIN_TBL_WIDTH+'>';
    copyright += '<tr><td class="copyright">';
    copyright += '<br>'+copyrightString+'<br><br>';
    copyright += '</td></tr></table>';
    
    return copyright;
}

/* =======================================================================
    Adjust div positions in case one of the refresh options is 'Off'.
    This keeps the screen looking good.
*/
function positionOffText()
{
    var offset = 45;
    
    for (var ii = 0; ii < 3; ++ii) 
    {
        var str = "" + refreshRates[ii];

        document.getElementById("refresh"+ii+"Div").style.left = offset + "px";
        if (refreshRates[ii] == 0) 
            offset += 22;
        else if (str.length > 1) 
        {
            offset += 10;
            for (var jj = 0; jj < str.length - 1; ++jj)
                offset += 8;
        }
        else
            offset += 10;
    }
}

/* =======================================================================
*/
function NavigationHead(rightCol)
{
    var container;
    init();
    
    document.write('<DIV ID="settingsContainer">');
    document.write('<div id="refreshDiv" class="settings">Refresh:');
    document.write('<div id="refresh0Div">&nbsp;<a id="refresh0A" href="javascript:void(0)" onClick="setRefreshRate(0)" class="settings">');
    document.write(refreshRates[0] ? refreshRates[0] : 'Off');
    document.write('</a></div>');
    document.write('<div id="refresh1Div">&nbsp;<a id="refresh1A" href="javascript:void(0)" onClick="setRefreshRate(1)" class="settings">');
    document.write(refreshRates[1] ? refreshRates[1] : 'Off');
    document.write('</a></div>');
    document.write('<div id="refresh2Div">&nbsp;<a id="refresh2A" href="javascript:void(0)" onClick="setRefreshRate(2)" class="settings">');
    document.write(refreshRates[2] ? refreshRates[2] : 'Off');
    document.write('</a></div></div>');
    document.write('<div id="logoutDiv" class="settings"><a id="Logout" href="javascript:void(0)" onClick="logoutClick()" class="settings">Logout</a></div>');
    document.write('</DIV>');
    
    // Header
    document.write('<table border=0 cellspacing=0 cellpadding=0 width='+MAIN_TBL_WIDTH+'>');
    document.write('<tr><td width='+parseInt(MAIN_TBL_MARGIN+MAIN_TBL_MARGIN)+'>&nbsp;</td><td align=center colspan=3><br><a target="_blank" href="http://www.raima.com"><img src="raima.png" border="0" alt="If it\'s fast, it\'s Raima"></a></td></tr>');
    document.write('</table>');
    
    // beginning of body
    document.write('<div id="hiddenContainer"></div><div id="helpContainer"></div>');
    document.write('<div id="contentContainer"><div id="tabContainer"></div><div id="pageContainer"></div><br><br>');
        
    document.getElementById("refresh"+refreshIndex+"A").className="settings_on";
    
    container = document.getElementById('settingsContainer');
    container.style.left = (MAIN_TBL_WIDTH - container.offsetWidth - MAIN_TBL_MARGIN - MAIN_TBL_MARGIN) + "px";
    
    container = document.getElementById('contentContainer');
    container.style.left = MAIN_TBL_MARGIN + "px";
    container.style.width = MAIN_TBL_WIDTH + "px";
    
    container = document.getElementById('pageContainer');
    container.style.left = PAGE_MARGIN + "px";
    container.style.width = (MAIN_TBL_WIDTH - PAGE_MARGIN - PAGE_MARGIN) + "px";
    
    positionOffText();
}

/* =======================================================================
*/
function NavigationTail()
{
    document.write('<br></div>');
    document.write('<br>' + getCopyright());
}



/* =======================================================================
*/
function getAjaxObject(fcn)
{
    var xmlhttp;
    
    if (window.XMLHttpRequest) 
    {
        // code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();
    }
    else if (window.ActiveXObject) 
    {
        // code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    else 
    {
        alert("Your browser does not support XMLHTTP!");
        return null;
    }
    
    return xmlhttp;
}

/* =======================================================================
    reqtype is GET or POST
    page is the URL
    fcn is the response callback function
    params are null for GET and name=value pairs for POST
    tab and userdata are 'expandos' on the xmlhttp object for use
    by the callback function.
*/
function callServer(reqtype, page, fcn, params, tab, userdata)
{
    var xmlhttp;
    
    xmlhttp = getAjaxObject(fcn);
    
    xmlhttp.open(reqtype, page, true);
    
    /* Define a response handler */
    /* IE (at least v8) does not work properly when specifying onreadystatechange
       with a function call.  (xmlhttp.onreadystate = fcn)
       The function was called, but this.readyState was never getting to 4.
       Creating the function here and passing in the xmlhttp object works, as
       function(req), the function reads req.readyState instead of this.readyState.
     */
    xmlhttp.onreadystatechange = function() {fcn(xmlhttp)};
    
    xmlhttp.setRequestHeader('If-Modified-Since', 'Thu, 1 Jan 1970 00:00:00 GMT');
    xmlhttp.setRequestHeader('Cache-Control', 'no-cache');

    xmlhttp.tab = tab;
    xmlhttp.userdata = userdata;
   
    if (params)
    {
        xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
        xmlhttp.setRequestHeader("Content-length", params.length);
    
        xmlhttp.send(params);
    }
    else
        xmlhttp.send(null);
    
    return xmlhttp;
}

