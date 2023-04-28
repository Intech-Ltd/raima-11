//
//  objcapi_private.h
//

#ifndef objcapi_private_h
#define objcapi_private_h
#import "objcapi.h"

@interface RDM_TFS (RDM_TFS_Private)

//return an NSString of the RDM error code given 
-(NSString *)descriptionFromErrCode:(int32_t)code;

//populate an error object with code and description
-(void)populateError:(NSError **)error withDescription:(NSString *)desc code:(int32_t)code;

@end

#pragma mark Private Database Helper Methods

@interface RDM_Db (RDM_Db_Private)

#pragma mark Open Methods
//open the task
-(int32_t)openTask;

//open the database
-(int32_t)openDbWithMode:(mode)mode name:(NSString *)name;

//open the database
-(BOOL)openDb:(NSString *)name mode:(mode)mode dbdArray:(const void*)dbdArray dbdSize:(int32_t)dbdSize error:(NSError **)error;

//finish the open
-(int32_t)finishOpen;

#pragma mark Locking Methods
//get locks for requested record(s)
-(int32_t)getLocks:(int32_t [])recTypes recCount:(int16_t)recCount lockType:(char)lockType;

#pragma mark Error Handling Methods

//return an NSString of the RDM error code given 
-(NSString *)descriptionFromErrCode:(int32_t)code;

//populate an error object with code and description
-(void)populateError:(NSError **)error withDescription:(NSString *)desc code:(int32_t)code;

@end


#pragma mark - Private Cursor Helper Methods
@interface RDM_Cursor (RDM_Cursor_Private)

typedef enum {
    kWriteLock,
    kReadLock
} LOCK_TYPE;


#pragma mark Set Methods
//lock a set
-(BOOL)lockSet:(int32_t)set withType:(LOCK_TYPE)type error:(NSError **)error;

//get the first member of a set of specified owner
-(id)firstMemberOfSet:(int32_t)set owner:(DB_ADDR)owner member:(DB_ADDR *)member error:(NSError **)error;

//get the last member of a set with specified owner
-(id)lastMemberOfSet:(int32_t)set owner:(DB_ADDR)owner member:(DB_ADDR *)member error:(NSError **)error;

//get the next member of a set with specified owner
-(id)nextMemberOfSet:(int32_t)set owner:(DB_ADDR)owner member:(DB_ADDR *)member error:(NSError **)error;

//get the previous member of a set with specified owner
-(id)previousMemberOfSet:(int32_t)set owner:(DB_ADDR)owner member:(DB_ADDR *)member error:(NSError **)error;

//get the current member of a set 
-(id)currentMemberOfSet:(int32_t)set member:(DB_ADDR)member error:(NSError **)error;

//get the current owner of a set 
-(id)currentOwnerOfSet:(int32_t)set owner:(DB_ADDR)owner error:(NSError **)error;

//make the current record owner of set
-(BOOL)makeCurrentOwnerOfSet:(int32_t)set owner:(DB_ADDR *)owner member:(DB_ADDR *)member error:(NSError **)error;

//make the current record owner of set
-(BOOL)makeCurrentMemberOfSet:(int32_t)set owner:(DB_ADDR *)owner member:(DB_ADDR *)member error:(NSError **)error;

//remove the current record from the set
-(BOOL)removeFromSet:(int32_t)set owner:(DB_ADDR)owner member:(DB_ADDR *)member error:(NSError **)error;

//make the current record a member of set
-(BOOL)addToSet:(int32_t)set owner:(DB_ADDR)owner member:(DB_ADDR *)member error:(NSError **)error;

//get the total number of members of a set
-(NSInteger)totalMembersOfSet:(int32_t)set owner:(DB_ADDR)owner error:(NSError **)error;

-(void)cleanup;

#pragma mark Record Type Methods

//allocate buffer if necessary
-(int32_t)allocBufferToSize:(size_t)size;

//returns the record type of a given object
-(int32_t)recordTypeFromObject:(id)object;

//returns the record type of a given field
-(int32_t)recordTypeFromField:(int32_t)field;

#pragma mark Record - Object Conversion Methods

//convert record in buffer to an object
-(id)objectFromRecord:(int32_t)type;

//convert object to buffer
-(int32_t)recordFromObject:(id)object;

//write blob data if it exists in the object
-(int32_t)writeBlobsFromObject:(id)object;

#pragma mark Insert, Update, Retrieve Methods

//insert a record at specified address with data from specified object
-(BOOL)insertObj:(id)obj at:(DB_ADDR)addr error:(NSError **)error;

//insert a new record with data from specified object
-(BOOL)insertObj:(id)obj error:(NSError **)error;

//return the object with data from the record at specified address
-(id)recordAtAddr:(DB_ADDR)addr error:(NSError **)error;

//write blob data
-(int32_t)writeData:(NSMutableData *)data toBlobField:(int32_t)field;

//read blob data
-(int32_t)readData:(NSMutableData **)data fromBlobField:(int32_t)field;

@end

#endif
