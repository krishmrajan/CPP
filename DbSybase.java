package com.bnpparibas.cdor;
import java.io.*;
import java.util.*;
import java.sql.*;

import com.sybase.jdbc2.jdbc.SybConnection;
import com.sybase.jdbcx.SybDriver;

/** Creates a new instance of DatabaseConnection */
//public DatabaseConnection (String strServer, String strPort, String strUserName, String strPassword){
// DriverManager.registerDriver(new com.sybase.jdbc2.jdbc.SybDriver());
// Connection connr = Null;
// Connection.connr = Null;
// DriverManager.getConnection("jdbc:Sybase:Tds:" + strServer + ":" + strPort, strUserName, strPassword);

public class DbSybase {
	Connection con = null;
	static String user = "adm_gdu_rec";
	static String password = "adm_gdu_rec";
	static String server = "jdbc:sybase:Tds:tiths015:5480";
	
	public Connection connect() {
		try {
			Class.forName("com.sybase.jdbc2.jdbc.SybDriver").newInstance();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
		Properties props = new Properties();
		props.put("user", user);
		props.put("password",password);
		props.put("server", server);
		try {
			con = (SybConnection)
			      DriverManager.getConnection(server,props);
		} catch (SQLException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		DatabaseMetaData dma;
		try {
			dma = con.getMetaData();
			System.out.println("\nConnected to " + dma.getURL() + "\n");
	        System.out.println("Driver       " + dma.getDriverName() + "\n");
	        System.out.println("Version      " + dma.getDriverVersion() + "\n");
	        System.out.println("Version      " + dma.getDatabaseProductName());
	       } catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

        


		
		
		System.out.println("returning connection ");
		return con;
	} // connect
	public void disconnect() {
		if (con != null)
			try {
				con.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
	}
	
	public void CreateStruct(String TblName, Connection con2, String View) {
		Statement stmt = null;
		ResultSet rs = null;
		int colNul;
		String tmpcol = null;
		int numberOfColumns;
		try {
			 System.out.println("Table name " + TblName );
			String SQL = "SELECT * from " + TblName + " where 1 = 2 ";

			// System.out.println(SQL);
			// ////////////////////
			stmt = con2.createStatement();
			rs = stmt.executeQuery(SQL);
			ResultSetMetaData rsMetaData = rs.getMetaData();
			numberOfColumns = rsMetaData.getColumnCount();
			int i;
			String format = "          %1$-50s %2$-20s %3$-10s";

			format = "          %1$-50s %2$-50s";
			// System.out.println("No of columns for this result set is " +
			// numberOfColumns);
			// System.out.println("IF NOT EXISTS (SELECT * FROM
			// tempdb.dbo.sysobjects WHERE id = OBJECT_ID(N'[dbo].["+TblName +
			// "]')) ");
			// System.out.println(" BEGIN");
			if (View == "View") {
				System.out.println("CREATE VIEW  dbo.V_CDOR_" + TblName
						+ " \n AS \nSELECT \n");

				for (i = 1; i <= numberOfColumns; i++) {
					tmpcol = rsMetaData.getColumnName(i);
					String ex[] = { tmpcol, "As " + tmpcol };
					System.out.print(String.format(format, (Object[]) ex));
					if (i < numberOfColumns)
						System.out.println(",");
				}
				System.out.println("\nFROM  " + TblName + "\nGO\n");

			}

			else {
				format = "          %1$-50s %2$-20s %3$-10s";
				System.out.println("CREATE TABLE " + TblName + "(");
				String tst = "NULL";
				for (i = 1; i <= numberOfColumns; i++) {
					tmpcol = rsMetaData.getColumnName(i).replace(' ', '_');
					tmpcol = tmpcol.replace('.', '_');
					colNul = rsMetaData.isNullable(i);
					tst = "NULL";
					if (colNul == 0)
						tst = "NOT NULL";
					String colName = rsMetaData.getColumnTypeName(i);
					String coltyp = rsMetaData.getColumnTypeName(i);
					if (coltyp.equals("varchar") || coltyp.equals("char")) {
						String ex[] = {
								tmpcol,
								coltyp + "(" + rsMetaData.getPrecision(i) + ")",
								tst };
						System.out.print(String.format(format, (Object[]) ex));
						// System.out.println(" (" + rsMetaData.getPrecision(i)
						// + ")" );
					} else if (coltyp.equals("decimal")) {
						String ex[] = {
								tmpcol,
								coltyp + "(" + rsMetaData.getPrecision(i) + ","
										+ rsMetaData.getScale(i) + ")", tst };
						System.out.print(String.format(format, (Object[]) ex));
						// System.out.print("(" + rsMetaData.getPrecision(i) +
						// "," );
						// System.out.println( + rsMetaData.getScale(i) + ")" );
					} else {
						String ex[] = { tmpcol, coltyp, tst };
						System.out.print(String.format(format, (Object[]) ex));
					}
					if (i < numberOfColumns)
						System.out.println(",");
				}// end for
				System.out.println(')');
				System.out.println("GO");
				// System.out.println(" SET @syberror = @@error ");
				// System.out.println(" IF @syberror !=0 BEGIN SET @logmsg =
				// 'Error create ' GOTO GTRAN END");
				// System.out.println(" END\n");
			} // end for
			// /////////////////////
		}

		catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (rs != null)
				try {
					rs.close();
				} catch (Exception e) {
				}
			if (stmt != null)
				try {
					stmt.close();
				} catch (Exception e) {
				}
		}
	}	

}
