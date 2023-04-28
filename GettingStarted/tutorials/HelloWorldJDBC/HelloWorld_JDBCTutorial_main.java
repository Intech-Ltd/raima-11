/* ----------------------------------------------------------------------------
 *
 * Raima Database Manager
 *
 * Copyright (c) 2012 Raima Inc.,  All rights reserved.
 *
 * Use of this software, whether in source code format, or in executable,
 * binary object code form, is governed by the Raima LICENSE which
 * is fully described in the LICENSE.TXT file, included within this
 * distribution of files.
 *
 * ----------------------------------------------------------------------------
 */

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

/**
 * helloWorldJDBC: This example program shows the basic steps of creating
 * a JDBC application. This class performs the following.  Inserts data into 
 * the database, retrieves the data using a ResultSet object, which is then 
 * displayed out to the screen.This example uses the JNI connection URL. In order 
 * to use JDBC you must specify your connection URL as "jdbc:raima:rdm://localhost" 
 * instead of the JNI which is "jdbc:raima:rdm://local". The comments are javadoc
 * style for consistency.
 * @author Kevin Hooks
 * Date : 2-2-2012
 */
public class HelloWorld_JDBCTutorial_main {

	/**
	 * main: This method connects to the driver by using the connection URL
	 * through JNI (TCP/IP). The database is then opened and cleaned up. 
	 * The string "Hello World!" is then inserted into the database. Every 
	 * object is initialized to null so that it can be used inside of each
	 * block (try, catch, finally).
	 * @param args
	 * @throws SQLException 
	 */
	public static void main(String[] args) throws SQLException {
        /* Connection object used for connecting to driver */
		Connection Conn = null; 
		/* Statement object used for executing SQL Statements */
		Statement Stmt = null;
		/* Prepared Statement object used for preparing SQL Statements */
		PreparedStatement prepStmt = null;
		/* Result Set object used for storing queried values */
		ResultSet rs = null;
		/*
		 * The try block contains the main portion of the code. The code is contained
		 * inside here so that if it fails the exception will be caught inside the
		 * catch blocks.
		 */
		try
		{
			/* Connects to driver using connection URL and passed into Connection Object */
			Conn = DriverManager.getConnection("jdbc:raima:rdm://local");
			/* Creates the Statement object to be used for future SQL Statement executions */
			Stmt = Conn.createStatement();
			Stmt.executeUpdate("OPEN hello_worldJDBC"); //Opens database
			Stmt.executeUpdate("DELETE FROM hello_table"); //Used to clear table if there is previous data
			/* Prepares the insert statement for execution */
			prepStmt = Conn.prepareStatement("INSERT INTO hello_table (f00) VALUES (?)");
			/* Performs a setter method for a string setting the value of the parameter above */
			prepStmt.setString(1, "Hello World!");
			prepStmt.execute(); //Executes prepared Statement
			Conn.commit(); //Commits all changes
			/* Executes a select statement and stores queried results in the result set */
            rs = Stmt.executeQuery("SELECT * FROM hello_table");
			/* 
			 * Loops through the result set, checking for when the next value is null.
			 * We only expect 1 iteration. The contents of helloString are then received
			 * from the getter method. The results are then printed to the screen
			 */
			while(rs.next() != false)
			{
				String helloString = rs.getString(1);
				System.out.println(helloString);
			}
			
		}
		/*
		 * The catch block is used to catch an exceptions that are thrown above and 
		 * display them to the screen. Currently the only type of exception we can
		 * expect would be a SQLException otherwise we would have to have another
		 * catch block for each possible exception 
		 */
		catch(SQLException exception)
		{
			System.err.println("An exception has occured : " + exception.toString());
		}
		/*
		 * The finally block is used to execute after the try block has been performed.
		 * It is also executed if an exception is thrown, this way you can clean up any
		 * open objects without missing the .close() on a failure. Every object must be
		 * closed as you would a handle in ODBC. 
		 */
		finally
		{
			if(rs != null)
				rs.close();
	
			if(prepStmt != null)
				prepStmt.close();
			
			if(Stmt != null)
				Stmt.close();
			
			if(Conn != null)
				Conn.close();		
		}
	}

}
