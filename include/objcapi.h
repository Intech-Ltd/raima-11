/* ----------------------------------------------------------------------------
 * Raima Database Manager - Objective-C API
 *
 * Copyright (c) 2011 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 * ----------------------------------------------------------------------------
 */

#import <Foundation/Foundation.h>
#include "rdm.h"

#define RDM_OBJC_ERROR_DOMAIN @"com.raima.rdm.objc.ErrorDomain"

#pragma mark - TFS Class

/**
 The **Transactional File Server** (TFS), is the database "engine" of Raima Database Manager, and has three modes: Standalone (TFSS), Built-in (TFST), and Client (TFSR). The TFSS and TFST run as threads within your application, while TFSR is simply a client-only mode, which connects using TCP/IP to another TFS which is accepting connections.
 
 The TFS Class provides start up and configuration of the TFS from your application. The TFS type, document root, and whether to accept incoming connections can all be configured.
 
 
 While the TFSS and TFST both run from within an application, they have several main differences:
 
 - The TFST is fully ACID compliant, whereas the TFSS does not provide any recoverability and is not ACID.
 - The TFST can be configured to accept incoming database connections from other processes (themselves using TFSR) over TCP/IP, whereas the TFSS cannot. The TFST is thus multi-user, while the TFSS is restricted to exclusive access from a single thread in the application.
 
 As a tradeoff for better safety and multi-user ability, the TFSS provides the highest performance of any of the configurations. If your application has exclusive access to the database, and does not require full ACID capabilities, then this may be an appropriate choice.
 
 The other main configurable aspect of the TFS is the **document root**. The document root is the location of where database files will be physically located. This is only used with the TFST and TFSS configurations.
 
 By default, if no type is specified, the TFS type will default to TFST, the Built-in TFS. In addition, the current working directory of the application will be used for the document root. If these defaults are acceptable for the application, then the TFS does not need to be used.
 
 Database connections can be opened and closed without needing to do anything further with the tfs, however before your application terminates, it must call term: on the TFS object to shut it down.
 
 The TFS type can be changed inside an application, however all database connections _must_ be closed, and term: (and stop: if accepting remote connections) must be called prior. This could be useful if you have for example, a large amount of data to load, but once loaded, you require full ACID multi-user capability. The data could be loaded using TFSS for maximum performance (but when higher safety is not yet needed), then the configuration could be changed to use TFST for regular operation.
 
 Output information
 
 The property tfsLogLevel determines the amount of output written to the console, when using `TFS_TYPE_TFS` or `TFS_TYPE_STANDAONE`. The level `kNormal` will print out the **document root**, and, if the server is accepting incoming connections, the **port** (only on `TFS_TYPE_TFS`). In addition, errors on the TFS will also be shown. This is the default output level if none is specified.
 
 `kVerbose` will output the same as `kNormal`, plus all transaction information. It is not recommended to use this level outside of debugging purposes. The final level is `kSilent`, which produces no output to the console.
 
 These levels are defined in the enumerated typeq `logLevel`:
 
     typedef enum {
     kSilent,
     kNormal,
     kVerbose
     }logLevel;
 
 To specify the log level, simply set the property prior to calling `tfsInitOfType:error:`, `tfsInitOfType:docroot:error:`, or `tfsInitOfType:docroot:port:error:`
 
     tfs.tfsLog = kSilent;
     [tfs tfsInitOfType:TFS_TYPE_STANDALONE];
 
 ####Examples
 
 Using the TFSS, with the document root in the default location of the application's Documents directory:
 
     RDM_TFS *tfs = [[tfs alloc] init]; //allocate the tfs object
     [tfs tfsInitOfType:TFS_TYPE_STANDALONE]; //internally start the TFS as TFSS, using the Documents directory as default
     
     ... //application code
 
     [tfs term]; //When done with this tfs, it needs to be shutdown internally
 
 Using the TFSS, while specifying the document root location, then using TFST while accepting connections:
 
     RDM_TFS *tfs = [[tfs alloc] init];
     [tfs tfsInitOfType:TFS_TYPE_STANDALONE docroot:@"/Users/raima/database"]; //start the TFS as TFSS
     
     ... //application code to load data in the database
     
     [tfs term];
     
     [tfs tfsInitOfType:TFS_TYPE_TFS docroot:@"/Users/raima/database"]; //start the TFS as TFST, using the same document root
 
     ... //application code that needs ACID capabilities
 
     [tfs term];

 @warning **NOTE:** When on iOS, the TFS class _must_ be used when using TFST or TFSS. Internally, RDM uses the function `getcwd()` to get the current working directory if no location is specified, and this currently resolves to "/" on iOS. This of course is not writeable by any iOS applications, so one must be specified. If the TFS class is used, the document root will be automatically set to the application's **Documents** directory, if none is specified.
 
 */

typedef enum {
    kSilent, //No output
    kNormal, //Print docroot and port (if server is listening) - default
    kVerbose //Print all transactions
}logLevel;

@interface RDM_TFS : NSObject {
@private
    TFS_HANDLE handle;
    uint16_t tfs_done;
    BOOL acceptRemote;
@protected
    TFS_TYPE tfsType;
    NSString *tfsDocroot;
    NSUInteger tfsPort;
    logLevel tfsLogLevel;
}

@property(nonatomic) TFS_HANDLE handle;
@property(nonatomic) uint16_t tfs_done;

/**
 The current TFS_TYPE of the RDM_TFS object. Can be `TFS_TYPE_TFS`, `TFS_TYPE_STANDALONE`, `TFS_TYPE_RPC`.
 */
@property(nonatomic, readonly) TFS_TYPE tfsType;

/**
 The current document root of the RDM_TFS object.
 */
@property(nonatomic, readonly) NSString *tfsDocroot;

/**
 The current document port of the RDM_TFS object.
 */
@property(nonatomic, readonly) NSUInteger tfsPort;

/**
 The current logging level of this RDM_TFS object. Change this property prior to calling tfsInitOfType:error: to change the logging level.
 */
@property(nonatomic) logLevel tfsLogLevel;

#pragma mark Init Methods

/** @name Creating a TFS Object */

-(id)init;

+(RDM_TFS *)tfs;


/** @name Starting a TFS */

/**
 Start the TFS internally in Raima Database Manager
 
 This method will start the TFS internally in RDM. Specify the type as `TFS_TYPE_RPC` (TFSR), `TFS_TYPE_TFS` (TFST), or `TFS_TYPE_STANDALONE` (TFSS). The application's Document's directory (iOS), or the user's Documents directory (Mac OS X) will be used for the document root (TFST or TFSS only). If remote incoming connections are enabled with -run (TFST only), the default port of 21553 will be used.
 
 @param type The type of TFS to use.
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns YES on success, NO on failure.
 */
-(BOOL)tfsInitOfType:(TFS_TYPE)type error:(NSError **)error;
 
/**
 Returns an initialized tfs object, specifying the type and document root.
 
 Use this method to start a TFS, while specifying the document root. Valid TFS types are `TFS_TYPE_TFS` (TFST), `TFS_TYPE_STANDALONE` (TFSS), and `TFS_TYPE_RPC` (TFSR). Note that when TFSR is used (client mode), the `docroot` parameter is ignored.
 
 @param type The type of TFS
 @param docroot The document root to start a tfs at. When NULL, defaults to application's Documents directory (iOS), or the user's Documents directory (Mac OS X). Ignored when type is TFS_TYPE_RPC (TFSR).
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns YES on success, NO on failure.
 */
-(BOOL)tfsInitOfType:(TFS_TYPE)type docroot:(NSString *)docroot error:(NSError **)error;

/**
 Return an initialized tfs object, specifying all parameters.
 
 This method will initialize a tfs object, while starting the tfs engine in RDM.
 
 @param type The type of TFS to use
 @param docroot The document root to start a tfs at. When NULL, defaults to Documents directory. Ignored when in RPC (client) mode.
 @param port The port to accept incoming connections on. When 0, defaults to `TFS_DEFAULT_PORT` (21553).
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns YES on success, NO on failure.
 */
-(BOOL)tfsInitOfType:(TFS_TYPE)type docroot:(NSString *)docroot port:(NSUInteger)port error:(NSError **)error;

/** @name Getting Information About the TFS */

/**
 Get the version and type of the TFS.
 
 Parameters are passed in for output to obtain the version (major, minor), and tfs type of the current tfs being used.
 
 @param major The major version
 @param minor The minor version
 @param type The TFS type
 */
-(void)getVersionMajor:(uint16_t *)major minor:(uint16_t *)minor type:(TFS_TYPE *)type;

/** @name Accepting Incoming Connections */

/**
 Start accepting incoming TCP/IP connections
 
 Calling this method will allow incoming connections on the TCP/IP port specified in port (default is 21553). Only has an effect when using TFST.
 
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns YES on success, NO on failure.
 */
-(BOOL)runWithError:(NSError **)error;

/**
 Stop accepting incoming TCP/IP connections
 
 This method will stop accepting incoming connections, and only has an effect when using TFST.
 
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns YES on success, NO on failure
 */
-(BOOL)stopWithError:(NSError **)error;

/** @name Shutting Down the TFS */

/**
 Shut down the TFS
 
 This method will shutdown the tfs. You may then call -start to start the tfs again and change the TFS type.
 
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns YES on success, NO on failure.
 */
-(BOOL)termWithError:(NSError **)error;

@end


#pragma mark - Database Class

/**
 The Database Class represents a connection to a database. The database can be stored locally on a TFS (either `TFS_TYPE_STANDALONE` or `TFS_TYPE_TFS`), or remotely on a TFS using TCP/IP to access (`TFS_TYPE_RPC`). Your application does not use the RDM_Db class directly, instead it uses the generated subclass, which has been customized for your specific schema. This will take the form of `<database name>_Db`. Multiple databases can be used by the same application at the same time, however each one needs it's own RDM_Db object.
 
 This class contains methods to do various operations at the database level, such as opening and closing the database, starting and stopping transactions, and getting information about the database.
 
 ##Opening And Closing the Database
 
 The first operation with the database should be to open it. There are various ways to do this, depending on the desired operation. An existing database dictionary (DBD) file can be used (created outside the application by running ddlp, or from a previous run of the application), or if it does not exist at the document root, one will be created and initialized by the application.
  
 ###Database Opening Modes
 There are three modes for opening a database:
 
 - **Shared:** Multiple users (Database Objects) accessing the same database at the same time.
 - **Read-Only:** Read only mode- no modifications can be made to the database. Required when using distributed database functionality.
 - **Exclusive:** Use when only a single user/thread is accessing a database.
 
 These modes are defined in the enum `mode`:
 
     typedef enum {
     kShared,
     kReadonly,
     kExclusive
     }mode;
 
 ###Opening the Database
 
 The database open can be combined with the RDM_Db init method, by using initAndOpenWithError:. In this case, the default database name will be used, with the default open mode of `kShared`. The default database name is automatically taken from the schema, and set in the property dbName, and likewise with the mode in openMode. This default can be overridden at any time by your application, causing any subsequent call to a method that uses the default name and open mode to use the new "default".
 
 

 To open the database on an existing RDM_Db object, use openWithError:, openWithOpenMode:error:, openWithDbName:error:, or openWithDbName:mode:error:.
 
 Note that in order to access a remote database using the "`name@host`" syntax, the default database name must be overridden, or the openWithDbName:error: or openWithDbName:mode:error: methods must be used to explicitly define the name to use.
 
 **NOTE:** If the database does not exist at the current document root in use, it will be created and initialized automatically.
 
 ###Closing the Database
 
 The database will be automatically closed when the RDM_Db object is released (either explicitly or when using ARC), however it can also be closed explicitly by using the close method. It can then be re-opened using the same object.
 
 ##Operations on an Open Database
 
 When not currently in a transaction, the database may be initialized. This removes all data from the database, and restores all files to their initial state, recreating them if necessary. Use the method initializeWithError: to initialize the database.
 
 **NOTE:** Great care should be used with this method, as all data in the database will be erased.
 
 ###Querying the Database
 
 The database may be queried to obtain information at the database wide level. Currently, obtaining the total number of records of a specified type is supported with totalRecordsOfType:error:.
 
 ###Transaction Control
 
 It is always good practice to encapsulate multiple database operations within transactions, providing better performance and safety. However, the RDM Objective-C API will automatically encapsulate any database operation within it's own transaction, if one has not already been started prior to calling the operation. This feature is provided as an "ease of use" for becoming familiar with RDM, however in a production environment, it is highly recommended that your own transactions are used.
 
 **NOTE:** Transactions are not used with TFST (`TFS_TYPE_TFS`). Transaction methods can still be called, however they will have no effect, as this type of TFS is not ACID compliant.
 
 There are three types of transactions in the Objective-C API:
 
 - Write
 - Read
 - Snapshot
 
 **Write transactions** are used whenever any type of modifications to the database will be done. This includes inserting, updating, or deleting records, and adding records to or removing from a set. **Read transactions** are used for just about all other database operations, where the database is not actually modified, but data will be read. This ensures that the data being read is current, as a read transaction will block any other users from modifying the data while it is in progress.
 
 Finally, a **snapshot transaction** is similar to a read transaction, however it allows multiple users to take snapshots at the same time, even while another user is in a write transaction. This is possible because the user is seeing a _snapshot_ of the database from a given point in time. While this gives better performance in a multi-user environment, it is more resource intensive for the TFS to maintain snapshots, so it should be avoided in a single user environment, as it gives no benefit. Instead, use a read transaction.
 
 In addition to providing concurrency control, transactions provide safety with an ACID environment. Each transaction must be ended before a new one begins, and all changes to the database are not written until the end. So it is possible to **roll back** any operation within the transaction, until it has been ended.
 
 ####Starting Transactions
 
 Read and write transactions may be started in two ways, by either
 
 - automatically locking all records in the database with beginWriteWithError: or beginReadWithError:
 
 or
 
 - specifying the specific records to be locked with beginWriteWhileLocking:recCount:error: or beginReadWhileLocking:recCount:error:.
 
 The first methods should be sufficient in most cases, however for most optimized performance, lock only the records required by the operations within that transaction.
 
 Snapshots only have one option, beginSnapshotWithError:.
 
 Note that if any transaction is in progress, you cannot start another until the existing one has been ended.
 
 ####Aborting and Pre-Committing Transactions
 
 A transaction may be rolled-back so that any operations done within the transaction up to that point will have no effect. To abort a transaction, call abortTransactionWithError:.
 
 A transaction may be "pre-committed", where all I/O required to write changes is done, however the operations are still not fully committed until the end. Following this call to precommitTransactionWithError:, either abortTransactionWithError: or endTransactionWithError: should be called. _No updates or reads should be done on the database following this call_, until the transaction has ended.
 
 **NOTE:** Both abortTransactionWithError: and precommitTransactionWithError: are only valid with **read** and **write** transactions. Both methods will fail if called from within a snapshot.
 
 ####Ending Transactions
 
 To end a transaction, simply call endTransactionWithError:, regardless of the transaction type started. If succesful, this will commit all changes within the transaction.
 
 @warning **NOTE:** In order to provide for advanced operation using the Native `d_` C API, the database task can be accessed as a property on the database object. Care should be taken in using this, as cursors defined locally may not be aware of all operations done when accessing the task directly. Please consult the RDM reference manual for more information on the Native API.
 
     Hello_world_Db *db = [[Hello_world_Db alloc] init];
     d_open("test", "r", db.task);
 
 */

//transaction types
typedef enum {
    kReadTrans, //read trans
    kWriteTrans, //write trans
    kReadOnlyTrans, //read only trans
    kNoTrans //no transaction
}transType;

//db open modes
typedef enum {
    kShared, //shared
    kReadonly, //readonly
    kExclusive // exclusive
}mode;

@interface RDM_Db : NSObject {
    DB_TASK *task;
    NSString *dbName;
    mode openMode;
    BOOL writeBlobs;
    BOOL readBlobs;
@private
    transType currTrans;
    LOCK_REQUEST *lr;
}

/**
 The database task. This should only be accessed when using advanced RDM Native API functions that are not covered by the Objective-C API.
 */
@property (nonatomic) DB_TASK *task;

/**
 The database name. Will be initialized automatically when using init or initAndOpenWithError:. You can override this to access remote databases with the `name@host` syntax.
 */
@property (nonatomic, retain) NSString *dbName;

/**
 The open mode of the database, which are specified with the enumerated type `mode`.
 
 `typedef enum {
  kShared,
  kReadonly,
  kExclusive,
 }mode;`
 
 - `kShared`: Shared mode, allows for multi-user access either over TCP/IP, or by multiple RDM_Db objects in different threads. Not supported with `TFS_TYPE_STANDALONE`
 - `kReadonly`: Read-only mode, allows read access only.
 - `kExclusive`: Single user mode, allows access from a single RDM_Db object.
 
 */
@property (nonatomic, readwrite) mode openMode;

/**
 Whether to write blob data when inserting records. The `writeBlobs` property on the cursor will be initialized from this setting, but can be overridden per cursor. The default is NO.
 */
@property (nonatomic) BOOL writeBlobs;

/**
 Whether to read blob data when reading records. The `readBlobs` property on the cursor will be initialized from this setting, but can be overridden per cursor. The default is NO.
 */
@property (nonatomic) BOOL readBlobs;


#pragma mark Init Methods

/** @name Initializing a Database Object and Opening the Database */

/**
 Returns an initialized database object.
 
 This method will create and initialize a database object, ready to connect to a database.
 
 @returns An initialized database object. Returns nil on failure.
 */
-(id)init;

/**
 Returns an initialized database object, with error information.
 
 This method will create and initialize a database object, ready to connect to a database. Error information is provided through the NSError object.
 
 @param error An error object that contains the error if method fails. Use NULL if you don't want to check the error.
 @returns An initialized RDM_Db object using the specified docroot. Returns nil on failure.
 */
-(id)initAndOpenWithError:(NSError **)error;

/**
 Returns a database object.
 
 This method will create a database object, ready to connect to a database.
 
 @returns A database object. Returns nil on failure.
 */
+(RDM_Db *)database;

#pragma mark Open Methods

/** @name Opening a Database */

/**
 Open the database, using the default name and mode.
 
 This method will open the database using the current database name specified in the dbName property, and the current open mode specified in the openMode property.
 
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)openWithError:(NSError **)error;

/**
 Open the database, specifying the open mode. Uses the default database name.
 
 Specify the mode to open the database, either `kShared`, `kReadonly`, or `kExclusive`. This will override what is set in the openMode property of RDM_Db. Uses the database name set in dbName.
 
 @param mode The mode to open the database in.
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)openWithOpenMode:(mode)mode error:(NSError **)error;

/**
 Open the database, specifying the database name.
 
 Specify the database name to open. This will override what is set in the dbName property of RDM_Db. Uses the open mode set in openMode.
 
 @param name The database name. See d_open() for database name formats.
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)openWithDbName:(NSString *)name error:(NSError **)error;

/**
 Open the database, specifying the database name and open mode.
 
 Specify both the database and open mode. This will override what is set in the RDM_Db properties dbName and mode.
 
 @param name The database name. See d_open() for database name formats.
 @param mode The mode to open: `kShared`, `kReadonly`, or `kExclusive`.
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)openWithDbName:(NSString *)name mode:(mode)mode error:(NSError **)error;

/**
 Initialize the database
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)initializeWithError:(NSError **)error;

#pragma mark Transaction Methods

/** @name Starting and Ending Database Transactions and Snapshots */


/**
 Begin a write transaction.
 
 This method will place a write lock all records in the database, and begin a transaction to allow write operations. If you know the specific records that need to be locked, and require the most optimized performance, use `beginWriteWhileLocking:recCount:error:`, which will lock only the records specified.
 
 It will fail if any transaction is already active; with `S_TRACTIVE` if a write transaction is already active, or `S_ROTACTIVE` if a read or snapshot transaction is active.
 
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)beginWriteWithError:(NSError **)error;

/**
 Begin a write transaction, specifying the record types to lock.
 
 This method will place a write lock the record types specified in the array `records`, and begin a transaction to allow write operations. You must also pass in the size of the array to `recCount`.
 
 It will fail if any transaction is already active; with `S_TRACTIVE` if a write transaction is already active, or `S_ROTACTIVE` if a read or snapshot transaction is active. If no records are specified to be locked (the array is nil or count is 0), then all records will be locked (see beginWriteWithError:).
 
 @param records An array of record types to lock.
 @param count The number of elements in records array.
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)beginWriteWhileLocking:(int32_t [])records recCount:(int32_t)count error:(NSError **)error;

/**
 Begin a read transaction.
 
 This method will place a read lock on all records in the database, and begin a transaction to allow read operations. If you know the specific records that need to be locked, and require the most optimized performance, use beginReadWhileLocking:recCount:error:, which will lock only the records specified.
  
 It will fail if any transaction is already active; with `S_TRACTIVE` if a write transaction is already active, or `S_ROTACTIVE` if a read or snapshot transaction is active.
 
 @warning  While a read transaction will be less expensive on the TFS than a snapshot, it will block all other concurrent read or write transactions. If the application is multi-user, and concurrent reads are required, use a snapshot transaction instead.

 
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)beginReadWithError:(NSError **)error;

/**
 Begin a read transaction, specifying the record types to lock.
 
 This method will lock the record types specified in the array `records`, and begin a read transaction to allow read operations. You must also pass in the size of the array to `recCount`.
 
 It will fail if any transaction is already active; with `S_TRACTIVE` if a write transaction is already active, or `S_ROTACTIVE` if a read or snapshot transaction is active.  If no records are specified to be locked (the array is nil or count is 0), then all records will be locked (see beginReadWithError:).
 
 @warning  While a read transaction will be less expensive on the TFS than a snapshot, it will block all other concurrent read or write transactions. If the application is multi-user, and concurrent reads are required, use a snapshot transaction instead.
 
 @param records An array of record types to lock.
 @param count The number of elements in records array.
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)beginReadWhileLocking:(int32_t [])records recCount:(int32_t)count error:(NSError **)error;

/**
 Begin a read-only snapshot of the database.
 
 This method begins a snapshot transaction of the database. This will allow read operations on the database from the moment the snapshot is taken, while not blocking any other users to the same database (including writes).
 
 @warning While a snapshot is useful in a multi-user environment, allowing concurrent reads, it is more resource intensive on the TFS. In a single user environment, a read transaction should be used instead, as a snapshot provides no benefit.
 
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)beginSnapshotWithError:(NSError **)error;

/**
 End a transaction.
 
 Ends any type of transaction previously started, committing changes to the database (when applicable).
 
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)endTransactionWithError:(NSError **)error;

/**
 Abort a transaction.
 
 Will abort any changes made between the beginning of a write transaction.
 
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)abortTransactionWithError:(NSError **)error;

/**
 Pre-Commit a transaction.
 
 Prepares changes to be committed to the database, by handling all I/O operations. However, the changes can still be aborted at this point. Only valid on a write transaction.
 
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)precommitTransactionWithError:(NSError **)error;

#pragma mark Database Level Info Methods

/** @name Getting Information About a Database */

/**
 Get version information about the database
 
 Pass in pointers to strings to receive the version, build number (major.minor), and full version string (including build date) of the RDM runtime. Pass NULL for the values you do not want.
 
 @param version The version number (`[Major].[Minor]`)
 @param build The build number
 @param fullVersion The full version string (`[Product] [Major].[Minor].[Patch] Build [Build] [Date]`)
 */
-(void)getVersion:(NSString **)version build:(NSString **)build fullVersion:(NSString **)fullVersion;

/**
 Find the total number of records of a given type.
 
 @param type The record type to count
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns The total number of records, or -1 on failure. Check the error object for actual failure details.
 */
-(NSInteger)totalRecordsOfType:(int32_t)type error:(NSError **)error;

#pragma mark Cleanup & Memory Management Methods

/** @name Closing the Database */

/**
 Close the database.
 
 Closes the database. The database object is still valid, and can be re-used by subsequently calling one of the open methods. However, it is not required to use this method, as the database will be closed automatically when the object is released.
 */
-(void)close;


-(void)cleanup;


@end

#pragma mark - Cursor Class

/** 
 This class provides the capability to navigate a database, and add, edit, or delete records. Each cursor is generic, and can be used with any type of record in the database. Cursors store navigation data about the **current record**, that is, the current record where the cursor is pointing to. In addition, the cursor also stores the navigational information for navigating using sets, the **current owner** and **current member** records.
 
 When using the cursor to navigate, each method will not only position the cursor at the record, but will also return the record in object form. This eliminates the need to perform separate read operations, and can simply be ignored if the particular record's data is not needed.
 
 To insert new records or update existing records, a record object is passed in with the data to be inserted.
 
 Cursor types are generic, but navigation for each type is **not** stored separately in the cursor. Going from one record type to another will lose the status of the first type. To keep a cursor pointed to the original record type, create a second cursor to navigate the second type.
 
     Hello_world_cursor *crsr1 = [[Hello_world_cursor alloc] init];
     [crsr1 firstRecordOfType:PERSON];
     [crsr1 firstRecordOfType:CAR]; // No longer stores any info about PERSON, now pointing to CAR record
 
     [crsr1 firstRecordOfType:PERSON]; // Now pointing to a PERSON record again
 
     Hello_world_cursor *crsr2 = [[Hello_world_cursor alloc] initWithCursor:crsr1]; // Create second cursor pointing to same record as crsr1
     [crsr1 firstRecordOfType:CAR]; // crsr1 now points to CAR record, crsr2 still points to PERSON record
 
 ##Creating Cursors
 
 Cursors can be created either pointing at nothing using init: 
 
    Hello_world_Cursor *crsr = [[Hello_world_Cursor alloc] init];
 
 or from an existing cursor, pointing to the same record as the original cursor, using initWithCursor:
 
    [crsr firstRecordOfType:PERSON];
    Hello_world_Cursor *crsr2 = [[Hello_world_Cursor alloc] initWithCursor:crsr];

 
 ##Database Operations
 
 Database operations on an open database consist of several operations, including navigation and modification. Note that when a navigational operation is done, on success it will point the cursor at the record specified, and on failure the cursor will remain pointing where it was before the method was called. For example, if the cursor was pointing at the first record of the database, and a searchKey operation is done which returns no results, the cursor will continue to point to the first record after the call.
 
 ###Navigating Records
 
 Unlike SQL databases, RDM stores records in a certain order, and allows navigation based on this order. In addition, the order of keys and sets are also used for navigation. The cursor stores this information, known as it's **currency**.
 
 Note that record navigation operations require at least a read transaction or a snapshot to be in place (a write is also valid). However, if no transaction is active, the API will automatically create and end one for you. However, for best performance, it is highly recommended to group operations into transactions yourself wherever possible.
 
 ####Navigation Using Record Position
 
 If a new cursor is created using the initWithDatabase: method, the properties storing navigational data will be pointing at no record (`NULL_DBA`). To begin navigating using the cursor position, you must call firstRecordOfType:error: or lastRecordOfType:error:, followed by nextRecordOfType:error: or previousRecordOfType:error:. Each of these methods will return the record at that position (if it exists), and the **current record** will then point to that location.
 
    Hello_world_Cursor *crsr = [[Hello_world_Cursor alloc] init];
    Hello_world_Person *person = [[Hello_world_person alloc] init];
 
    [db beginReadWithError:&error];
    //read the first record
    person = [crsr firstRecordOfType:PERSON error:&error];
    NSLog(@"%@\n", person.name);
 
    //continue reading next records until there are no more
    while (person != nil) {
       person = [crsr nextRecordOfType:PERSON];
       NSLog(@"%@\n", person.name);
    }
    [db endTransactionWithError:&error];
    NSLog(@"No more person records\n");
 
 ####Navigation Using Keys
 
 You can navigate with a cursor using keys by using firstRecordOfKey:error:, lastRecordOfKey:error:, nextRecordOfKey:error:, and previousRecordOfKey:error:. This navigation is similar to using the record position, however the records are filtered by **key** instead of record type.
 
    Hello_world_Cursor *crsr = [[Hello_world_Cursor alloc] init];
    Hello_world_Person *person = [[Hello_world_person alloc] init];
 
    [db beginReadWithError:&error];
    //read the first record
    person = [crsr firstRecordOfKey:SSN error:&error];
    NSLog(@"%@\n", person.name);
 
    //continue reading next records until there are no more
    while (person != nil) {
       person = [crsr nextRecordOfKey:SSN];
       NSLog(@"%@\n", person.name);
    }
    [db endTransactionWithError:&error];
    NSLog(@"No more person records\n");
 
 Keys can be searched, using different methods depending on the data type of the data to be searched. If the search is successful and points to a record, calling nextRecordOfKey:error: and previousRecordOfKey:error: will return the record with the next higher or lower key value.
 
 Methods taking specific types are provided for the most common types, for all others a method which takes a `void *` is provided.
 
 - `NSString`: use `searchKey:withString:error:`
 - `NSInteger`: use `searchKey:withInteger:error:`
 - `double`: use `searchKey:withDouble:error:`
 - `DB_ADDR`: use `searchKey:withAddress:error:`
 - For all others, use `searchKeyWithPointer:error:`
 
 Example:
 
    Hello_world_Cursor *crsr = [[Hello_world_Cursor alloc] init];
    Hello_world_Person *person = [[Hello_world_person alloc] init];
 
    [db beginReadWithError:&error];
    //read the first record
    person = [crsr searchKey:SSN withInteger:1234 error:&error];
    if (person != nil) {
       NSLog(@"%@\n", person.name);
       person = [crsr nextRecordOfKey:SSN error:&error];
    }
    [db endTransactionWithError:&error];
 
 #####Partial Keys
 
 Cursors also support the use of partial keys, through searches and navigation.
 
 To search a partial key, use searchPartialKey:ofFields:stringLength:data:error:, and then navigate using nextPartialKey:ofFields:stringLength:data:error: or previousPartialKey:ofFields:stringLength:data:error:.
 
 ####Navigation Using Sets
 
 A unique feature of RDM's network model is the use of set relationships to organize data. Sets have the concept of **owner** and **members**. Specific record types are designated as **owners** and **members** in the database schema. During database operation, the specific _record_ which is the current owner will change, this concept is the **current owner**. As records are added, they must be connected to the specific set as members.
 
 The general process of adding records to a set is:
 
 1. Point the cursor to a specific record to be the **owner** of the set
 2. Designate that record as a current owner of a set
 3. Insert a new record, adding it to the set
 
 Example:
 
    person = [crsr firstRecordOfType:&error];
    [crsr makeOwnerOfSet:DRIVES error:&error];
    if ([crsr insertRecordFromObject:person2 error:&error]) {
       [crsr makeMemberOfSet:DRIVES error:&error];
    }
 
 For example, to find all the cars driven by a person with ssn of "1234", we
 
 1. Locate the owner record of the set
 2. Make the person the current owner of the DRIVES set
 3. Set the first member of the set; loop through each member record
 
 Example:
 
    person = [crsr searchKey:SSN withInteger:1234 error:&error];
    [crsr makeOwnerOfSet:DRIVES];
    car = [crsr firstMemberOfSet:DRIVES error:&error];
    while (car != nil) {
       NSLog("%d\n", car.vin);
       car = [crsr nextMemberOfSet:DRIVES error:&error];
    }
 
 ###Modifying the Database
 
 Modifying the database consists of these main operations, requiring a **write transaction**:
 
 - Inserting new records
 - Updating existing records
 - Deleting existing records
 
 In addition, modifying sets by adding or removing members (as described above), is also considered modifying the database and requires a write transaction.
 
 ####Inserting New Records
 
 To insert a new record, use the method insertRecordFromObject:error:, passing in a record object containing the data you want to insert.
 
    person.name = @"Firstname";
    person.ssn = 456;
    [crsr insertRecordFromObject:person error:&error];
 
 ####Updating Existing Records
 
 Existing records can be updated using the updateRecordFromObject:error: method. This method works the same as insertRecordFromObject:error:, except that instead of inserting a new record, the current record the cursor points to is updated with the data from the record object passed in.
 
    [crsr firstRecordOfType:PERSON error:&error];
    
    person.name = @"Othername";
    person.ssn = 789;
    [crsr updateRecordFromObject:person error:&error];
 
 ####Deleting Records
 
 Records can be deleted by pointing the cursor to the record to be deleted, then calling the deleteRecordWithError: method. Note that if the record is currently a member of a set, it must first be removed from the set by calling removeFromSet:error:.
 
    [crsr lastRecordOfType:CAR error:&error];
    [crsr removeFromSet:DRIVES error:&error];
    [crsr deleteRecordWithError:&error];
 
 ###Blobs
 
 Blobs will be read and written according to the cursor properties writeBlobs and readBlobs, which are both set to the database writeBlobs and readBlobs properties by default. You can override this either at the database wide level, or per cursor.
 
 ###String Truncation
 
 Internally, RDM stores strings as fixed length `char` arrays. Since the Objective-C API will translate these into `NSString` objects, which are not fixed length, it is possible that an object could contain a string which exceeds the length of the internally stored data type. To handle this, two options are available: **truncate** the strings automatically to a safe length (the default), or **fail** with an error if an object with a longer string is attempted to be inserted. This is set in the property truncateStrings.
 
 ###Binary Array Truncation
 
 Internally, RDM stores inline binary data as fixed length `char` arrays. Since the Objective-C API will translate these into `NSMutableData` objects, which are not fixed length, it is possible that an object could contain data which exceeds the length of the internally stored data type. To handle this, two options are available: **truncate** the data automatically to a safe length (the default), or **fail** with an error if an object with too much data is attempted to be inserted. This is set in the property truncateData.
 
 */

@interface RDM_Cursor : NSObject {
@private
    RDM_Db *db;
    void *recBuff;
    int32_t size;
    DB_ADDR currRec;
@protected
    BOOL readBlobs;
    BOOL writeBlobs;
    BOOL truncateStrings;
    BOOL truncateData;
    DB_ADDR *currOwners;
    DB_ADDR *currMembers;
}

@property (nonatomic, retain)RDM_Db *db;
@property (nonatomic)void *recBuff;
@property (nonatomic)int32_t size;
@property (nonatomic)DB_ADDR *currOwners;
@property (nonatomic)DB_ADDR *currMembers;

/**
 Read blobs when retrieving records from the database.
 
 Inherits from the setting on the RDM_Db object (which is `NO` by default).
 */
@property (nonatomic)BOOL readBlobs;
/**
 Write blobs when inserting or updating records to the database.
 
 Inherits from the setting on the RDM_Db object (which is `NO` by default).
 */
@property (nonatomic)BOOL writeBlobs;
/**
 Whether to truncate strings if the length in the record object exceeds the length stored internally, or fail with an error.
 
 Default is `YES`.
 */
@property (nonatomic)BOOL truncateStrings;
/**
 Whether to truncate binary data if the length in the record object exceeds the length stored internally, or fail with an error.
 
 Default is `YES`.
 */
@property (nonatomic)BOOL truncateData;

#pragma mark Cursor Creation Methods

/** @name Creating Cursors */

/**
 Return an initialized cursor object for the specified database.
 
 All naviagational data points to NULL_DBA.
 
 @param database The database to use the cursor with.
 @returns An initialized cursor object.
 */
-(id)initWithDatabase:(RDM_Db *)database;

/**
 Return an initialized cursor object from an existing cursor.
 
 All navigational data points to the same as the existing cursor.
 
 @param cursor The existing cursor to copy from.
 @returns A new cursor object pointing to the same record as the exisitng cursor.
 */
-(id)initWithCursor:(RDM_Cursor *)cursor;

/**
 Return a cursor object for the specified database.
 
 All naviagational data points to NULL_DBA.
 
 @param database The database to use the cursor with.
 @returns A cursor object.
 */
+(RDM_Cursor *)cursorWithDatabase:(RDM_Db *)database;

#pragma mark Inserting and Updating Methods

/** @name Inserting, Updating, and Deleting Records */

/**
 Insert a new record with data from the specified record object.
 
 This method will take a record object and insert it's data into a new record in the database. The input object can be any valid Record Object in your schema (defined in `<database name>_objc.h`).
 
 If `object` is nil or not a valid Record Object, the operation will fail with the code S_INVREC.
 
 @param object A record object containing the data to be inserted in a new record.
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns `YES` on success, `NO` on failure.
 */
-(BOOL)insertRecordFromObject:(id)object error:(NSError **)error;

/**
 Update the current record where the cursor is positioned with the specified record object.
 @param object A record object containing the data to update the current record with.
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns `YES` on success, `NO` on failure.
 */
-(BOOL)updateRecordFromObject:(id)object error:(NSError **)error;

/**
 Delete the current record
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)deleteRecordWithError:(NSError **)error;

#pragma mark Record Navigation Methods

/** @name Navigating by Record */

/**
 Get the current record where the cursor is positioned.
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns An object containing the record data from the record at the specified type, or `nil` on failure.
 */
-(id)currentRecordWithError:(NSError **)error;

/**
 Get the type of the current record where the cursor is positioned.
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns The type of the record currently pointed to by this cursor object, or -1 on failure.
 */
-(int32_t)typeOfCurrentRecordWithError:(NSError **)error;


/** 
 Position the cursor at the first record of specified type, and return an object containing the record data.
 @param type The record type
 @param error The status will be set in this object (may be NULL if not used)
 @returns An object containing the record data, or `nil` on failure.
 */
-(id)firstRecordOfType:(int32_t)type error:(NSError **)error;

/**
 Position the cursor at the next record of specified type, and return an object containing the record data.
 @param type The record type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containing the record data, or `nil` on failure.
 */
-(id)nextRecordOfType:(int32_t)type error:(NSError **)error;

/**
 Position the cursor at the previous record of specified type, and return an object containing the record data.
 @param type The record type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containing the record data, or `nil` on failure.
 */
-(id)previousRecordOfType:(int32_t)type error:(NSError **)error;

/**
 Position the cursor at the last record of specified type, and return an object containing the record data.
 @param type The record type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containing the record data, or `nil` on failure.
 */
-(id)lastRecordOfType:(int32_t)type error:(NSError **)error;

/**
 Position the cursor at the position from another cursor.
 
 The current owner and member data is unchanged.
 @param cursor The cursor to point the current cursor to.
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containing the record data from the position of the cursor passed in.
 */
-(id)recordAtCursor:(RDM_Cursor *)cursor error:(NSError **)error;



#pragma mark Key Navigation Methods

/** @name Navigating by Key */

/**
 Position the cursor at the first key of specified type, and return an object containing the record data.
 @param key The key type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containing the record data from the record at the specified key, or `nil` on failure.
 */
-(id)firstRecordOfKey:(int32_t)key error:(NSError **)error;

/**
 Position the cursor at the next key of specified type, and return an object containing the record data.
 @param key The key type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containing the record data from the record at the specified key, or `nil` on failure.
 */
-(id)nextRecordOfKey:(int32_t)key error:(NSError **)error;

/**
 Position the cursor at the previous key of specified type, and return an object containing the record data.
 @param key The key type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containing the record data from the record at the specified key, or `nil` on failure.
 */
-(id)previousRecordOfKey:(int32_t)key error:(NSError **)error;

/**
 Position the cursor at the last key of specified type, and return an object containing the record data.
 @param key The key type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containing the record data from the record at the specified key, or `nil` on failure.
 */
-(id)lastRecordOfKey:(int32_t)key error:(NSError **)error;

/**
 Check if specified optional key is in the database
 @param field The field type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns YES if a key is present, NO if it does not. If NO, check error object to see if a failure occured.
 */
-(BOOL)keyExistOfField:(int32_t)field error:(NSError **)error;

/**
 Add a key entry from the data in the optional key `field` of the current record.
 
 Once stored, an optional key is automatically modified whenever its field contents are modified in the data file.
 @param field The field type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns YES if the key was stored successfully, NO on failure. If NO, check error object to see if a failure occured.
 */
-(BOOL)storeKeyOfField:(int32_t)field error:(NSError **)error;

/**
 Search keys with pointer to specified data.
 @param key The key field
 @param data Pointer to the data to search for
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containing record at matching key, or `nil` on failure.
 */
-(id)searchKey:(int32_t)key withPointer:(const void *)data error:(NSError **)error;

/**
 Search keys for specified string
 @param key The key field
 @param data String to search for
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containing record at matching key, or `nil` on failure.
 */
-(id)searchKey:(int32_t)key withString:(NSString *)data error:(NSError **)error;

/**
 Search keys for specified integer
 @param key The key field
 @param data Integer value to search for
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containing record at matching key, or `nil` on failure.
 */
-(id)searchKey:(int32_t)key withInteger:(NSInteger)data error:(NSError **)error;

/**
 Search keys for specified double
 @param key The key field
 @param data Double value to search for
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containing record at matching key, or `nil` on failure.
 */
-(id)searchKey:(int32_t)key withDouble:(double)data error:(NSError **)error;

/**
 Search keys for specified database address
 @param key The key field
 @param data Database address to search for
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containing record at matching key, or `nil` on failure.
 */
-(id)searchKey:(int32_t)key withAddress:(DB_ADDR)data error:(NSError **)error;

/**
 Search for a record by partial key
 @param key The key field to search
 @param fields The number of fields to search
 @param stringLength The partial string length in characters.
 @param data A pointer to the data to be searched.
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns An object containing the record matching the specified key info, or `nil` on failure.
 
 @warning When using this method on a key field of wchar_t type, `data` should be as a wchar_t character array or string.
 */
-(id)searchPartialKey:(int32_t)key ofFields:(NSUInteger)fields stringLength:(NSUInteger)stringLength data:(const void *)data error:(NSError **)error;

/**
 Get the next record by partial key
 @param key The key field to search
 @param fields The number of fields to search
 @param stringLength The partial string length in characters.
 @param data A pointer to the data to be searched.
 @param error An error object which contains error details on failure (pass in NULL if not used)
 */
-(id)nextRecordOfPartialKey:(int32_t)key ofFields:(NSUInteger)fields stringLength:(NSUInteger)stringLength data:(const void *)data error:(NSError **)error;

/**
 Get the previous record by partial key
 @param key The key field to search
 @param fields The number of fields to search
 @param stringLength The partial string length in characters.
 @param data A pointer to the data to be searched.
 @param error An error object which contains error details on failure (pass in NULL if not used)
 */
-(id)previousRecordOfPartialKey:(int32_t)key ofFields:(NSUInteger)fields stringLength:(NSUInteger)stringLength data:(const void *)data error:(NSError **)error;


#pragma mark Set Navigation Methods

/** @name Navigating By Set */

/**
 Position the cursor at the first member of the specified set, and return an object containing the record data.
 @param set The set type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containihg record data from the specified set member, or `nil` on failure.
 */
-(id)firstMemberOfSet:(int32_t)set error:(NSError **)error;

/**
 Position the cursor at the next member of the specified set, and return an object containing the record data.
 @param set The set type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containihg record data from the specified set member, or `nil` on failure.
 */
-(id)nextMemberOfSet:(int32_t)set error:(NSError **)error;

/**
 Position the cursor at the previous member of the specified set, and return an object containing the record data.
 @param set The set type
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns An object containihg record data from the specified set member, or `nil` on failure.
 */
-(id)previousMemberOfSet:(int32_t)set error:(NSError **)error;

/**
 Position the cursor at the last member of the specified set, and return an object containing the record data.
 @param set The set type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containihg record data from the specified set member, or `nil` on failure.
 */
-(id)lastMemberOfSet:(int32_t)set error:(NSError **)error;

/**
 Position the cursor at the current member of the specified set, and return an object containing the record data.
 @param set The set type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containihg record data from the specified set member, or `nil` on failure.
 */
-(id)currentMemberOfSet:(int32_t)set error:(NSError **)error;

/**
 Position the cursor at the current owner of the specified set, and return an object containing the record data.
 @param set The set type
 @param error An error object which contains error details on failure (pass in NULL if not used).
 @returns An object containihg record data from the specified set member, or `nil` on failure.
 */
-(id)currentOwnerOfSet:(int32_t)set error:(NSError **)error;

/**
 Set the current owner of the specified set to the current record
 @param set The set type
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)makeCurrentOwnerOfSet:(int32_t)set error:(NSError **)error;

/**
 Set the current member of the specified set to the current record
 @param set The set type
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)makeCurrentMemberOfSet:(int32_t)set error:(NSError **)error;

/**
 Check if record where cursor is positioned is the owner of specified set
 @param set The set type
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES if current record is an owner of the set, NO on if not or other error
 */
-(BOOL)isOwnerOfSet:(int32_t)set error:(NSError **)error;

/**
 Check if record where cursor is positioned is a member of specified set
 @param set The set type
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES if current record is a member of the set, NO on if not or other error
 */ 
-(BOOL)isMemberOfSet:(int32_t)set error:(NSError **)error;

/**
 Make the current record a member of the specified set
 @param set The set to make the current record a member of
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */ 
-(BOOL)addToSet:(int32_t)set error:(NSError **)error;

/**
 Remove the current record from the specified set
 @param set The set to remove the current record from
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns YES on success, NO on failure
 */
-(BOOL)removeFromSet:(int32_t)set error:(NSError **)error;

/**
 Get the total number of members in the set.
 @param set The set to get the count of members of.
 @param error An error object which contains error details on failure (pass in NULL if not used)
 @returns The total number of members, or `-1` on failure.
 */
-(NSInteger)totalMembersOfSet:(int32_t)set error:(NSError **)error;


@end

