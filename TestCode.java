package com.bnpparibas.cdor;
import java.sql.Connection;
public class TestCode {
	   public static void main(String[] args) {
		     // System.out.println("test");
		   //DbSybase dbSyb = new DbSybase();
		   //Connection con2 = dbSyb.connect();
		   	    
		    //dbSyb.CreateStruct("FUND",con2,"View");
		    Db db = new Db();
		   	Connection con1 = db.connect();             // tr_id    ps_id   ts_id , deal_test_id
		   	
		   //	db.ExcuteQuery("accountdetails", con1);
		   	db.disconnect();
		   //	db.CreateProc("(call CDOSUITE.dbo.PS_CT_RUN ( '378369',null, null,null,'DISP','8','1' ))",con1, "##CDOR_TESTS_RESULTS_SUMMARY", "CDOR_ALIM_CDOR_TESTS_RESULTS_SUMMARY");
		   //db.ExcuteQuery("CDOSUITE.dbo.Issue_Payment",con1);
		   	
		     //db.CreateProc("{call CDOSUITE.dbo.PS_Display @ps_id = 23941, @display_summary = 2,@ps_display_option = 'ISSU',@ps_layout_id = 410}",con1, "##CDOR_CT_Run", "CDOR_CT");
		   	//db.CreateProc("{call PS_Display ('23941','ISSU','410','BAL',null,null,null,'ASC','2')}",con1,"##Cdor_Ps_display" , "cdor_ps_display");
		   	/*@ps_id int,
			@ps_display_option lookup_code,
			@ps_layout_id int,   layout id to use to display 
			@pb_calc_method varchar(4) = 'BAL',
			@ts_id int = NULL,  trading scenario id 
			@filter_exp_id int = NULL,
			@order_field int  = NULL,  sequential column number to sort by 
			@order_direction char(4) = 'ASC',
				 ASC = Ascending;
				DESC = Descending 
			@display_summary tinyint = 1*/


		     //db.CreateProc("{call dbo.PS_Display ('24931', null ,'ITEM', 'BM1',NULL,440,'DEFAULT','DEFAULT')}",con1, "##CDOR_ps_display", "AMORTI");
		     
		     
		   	//db.ExcuteQuery("select * from account_transaction where account_trans_id  = 54610 or account_trans_id = 54609",con1);
		   	
			//Test for fred 
		   	//db.CreateProc("{call PS_CT_Run ( '378369' , null, null , null , 'DISP', '8')}",con1, "##CDOR_CT_Run", "CDOR_CT");
		   	//db.CreateProc("{call PS_CT_Run ( '228962' , null, null , null , 'DISP', '3')}",con1, "##CDOR_CT_Run", "CDOR_CT");
		   	/*EXEC CDOBP2SDEV.dbo.PS_CT_RUN
			@tr_id= 229198, --@tr_id, 		-- identifier of the test result
			@run_mode = 'DISP', 		-- mode run : SAVE, FULL, DISP
			@recordset_number = 3		-- record of the table in the multi recordset.*/
		   	
		   	//select * from Hedge
		   	//db.CreateStruct("HEDGE",con1,"");
		   	//db.CreateStruct("HEDGE",con1,"View");
		   	
		   //db.CreateStruct("##CDOR_DEAL_ISSUE_SERVICER",con1,"");
		   //db.CreateStruct("##CDOR_DEAL_ISSUE_SERVICER",con1,"View");
		   	
		   	//db.CreateStruct("CDOR_RPT_TRANSACTION_HISTORY",con1,"View");
		   	/*db.CreateStruct("Hedge_Payment_Int_Rate_History",con1,"");
		   	db.CreateStruct("Hedge_Payment_Notional_Amount_History",con1,"");
		   	db.CreateStruct("Hedge_Payment_Schedule",con1,"");
		   	db.CreateStruct("Hedge_Payment_Strike_Rate_Schedule",con1,"");
		   	
		   	db.CreateStruct("Hedge_Payment_Int_Rate_History",con1,"View");
		   	db.CreateStruct("Hedge_Payment_Notional_Amount_History",con1,"View");
		   	db.CreateStruct("CDOR_CDOBIZ_RATING",con1,"View");
		   	db.CreateStruct("Hedge_Payment_Strike_Rate_Schedule",con1,"View");*/
		   	//db.CreateStruct("dbo.issue_Servicer_view",con1,"");
		   	//db.CreateStruct("CDOR_DEAL_CASH_ACCOUNT",con1,"View");
		   	//db.CreateStruct("CDOR_CDOBIZ_SP_SECTOR",con1,"View");
		   	
		   	//db.CreateStruct("CDOR_ACCOUNT_TRANS_TYPE_MAP",con1,"View");
		   	//db.ExcuteQuery("CDOR_EXTRACTION_QUERY", con1);
		   	//db.CreateStruct("CDOR_CDOBIZ_DEFAULT_PROBABILITY_MATRIX",con1,"");
		   	//db.CreateStruct("CDOR_CDOBIZ_DEFAULT_PROBABILITY_MATRIX",con1,"View");
		   	//db.CreateStruct("CDOR_ISSUER_RATING",con1,"");
		   	//db.CreateStruct("CDOR_BANK_RATING",con1,"");
		   	//db.CreateStruct("##CDOR_CDOBIZ_INDUSTRY",con1,"");
		   	//db.CreateStruct("V_CDOR_TESTS_RESULTS_SUMMARY",con1,"View");
		   	
		   	//EXEC cdosuite.dbo.Deal_Industry_Correlation_list @matrix_id=1
		   	
		   //	Extraction_id
		   	//for (int i = 1; i <= 400 ; i++) {
		   	//	db.RefreshStatus(con1 , Integer.toString(i) );
					
			//} 
		   	
		   	 //db.CreateProc("{call test()}	",con1, "##CDOR_LAYOUT_406_ISSUER_2_test", "CDOR_ALIM_LAYOUT_406_ISSUER_2_test");
		   	//db.CreateStruct("##CDOR_CASH_TRANSACTION",con1,"View");
		   	 
		   	//db.CreateProcUpdate("{call test1()}	",con1, "##CDOR_RPT_Transaction_History", "CDOR_ALIM_RPT_Transaction_History");
		   	
		   	
		   //db.ExcuteQuery("##CDOR_SF_SCHEDULE", con1);
		   	//**************<KRA 10.04.2008> db.CreateProc("{call Deal_Industry_Correlation_list('1' , null)}	",con1, "##CDOR_CDOBIZ_DEFAULT_PROBABILITY_MATRIX", "CDOR_ALIM_CDOR_CDOBIZ_DEFAULT_PROBABILITY_MATRIX");
		   	//db.CreateProc("{call test ()}",con1, "##CDOR_ISSUE_TRANSACTION", "CDOR_ALIM_ISSUE_TRANSACTION");
		   	//db.CreateProcUpdate("{call test()}	",con1, "##CDOR_FACILITY_TRADE", "CDOR_ALIM_FACILITY_TRADE");
		   	//db.CreateProcUpdate("{call test3()}	",con1, "##CDOR_ISSUE_GLOBAL_TRANSACTION", "CDOR_ALIM_ISSUE_GLOBAL_TRANSACTION");
		   	//db.CreateStruct("##CDOR_FACILITY_TRANSACTION",con1,"View");
		   	//db.CreateStruct("CDOR_DEAL_ISSUE_LOT",con1,"View");
		   	//db.CreateProcUpdate("{call test3()}	",con1, "##CDOR_CDOBIZ_ISSUE_TRANS_TYPE", "CDOR_ALIM_CDOBIZ_ISSUE_TRANS_TYPE");
		   	//db.CreateProcUpdate("{call test1()}	",con1, "##CDOR_RPT_Transaction_History", "CDOR_ALIM_RPT_Transaction_History");
		   //	
		   //	db.CreateProc("{call cdobiz_Default_probability_matrix()}	",con1, "##CDOR_CDOBIZ_DEFAULT_PROBABILITY", "CDOR_ALIM_CDOR_CDOBIZ_DEFAULT_PROBABILITY");
		   	//db.ExuteQueryOutput(con1);
		   	
		   	//db.CreateProc("{call dbo.Deal_Servicer_list ('120', 'null')}",con1, "##CDOR_DEAL_SERVICER_LIST", "CDOR_ALIM_DEAL_SERVICER_LIST");
		   	//db.CreateProc("{call dbo.Bank_list ('null')}",con1, "##CDOR_BANK", "CDOR_ALIM_BANK");
		   	//db.CreateProcUpdate("{call dbo.Servicer_Rating_list('164','FR')}	",con1, "##CDOR_SERVICER_RATING", "CDOR_ALIM_SERVICER_RATING");
		   	
		   	
		   	//EXEC dbo.Servicer_Rating_list @rating_agency = 'FR', @bank_id = 164
		   	
		   	// 14.04.2009 db.CreateProc("{call cdosuite.dbo.PS_Test_Results_Get ('27047')}",con1, "##CDOR_TESTS_RESULTS_SUMMARY", "CDOR_ALIM_CDOR_TESTS_RESULTS_SUMMARY");
		   		   	
		   	
		   	//db.CreateProc("{call dbo.Hedge_Payment_get( '123', 'TC')}",con1, "##CDOR_HEDGE_PAYMENT", "CDOR_ALIM_HEDGE_PAYMENT");
		   	//db.CreateProc("{call dbo.Hedge_Payment_get( '123', 'TC')}",con1, "##CDOR_HEDGE_PAYMENT", "CDOR_ALIM_HEDGE_PAYMENT");
		   	//db.CreateProc("{call dbo.Hedge_Payment_Interest_rate_list( '123', 'TC')}",con1, "##CDOR_HEDGE_PAYMENT_IR_HISTORY", "CDOR_ALIM_HEDGE_PAYMENT_IR_HISTORY");
		   	//db.CreateProc("{call dbo.Hedge_Payment_Notional_amount_list( '123', 'TC')}",con1, "##CDOR_HEDGE_PAYMENT_NOTIONAL_AMOUNT_HISTORY", "CDOR_ALIM_HEDGE_PAYMENT_NOTIONAL_AMOUNT_HISTORY");
		   	//db.CreateProc("{call dbo.Hedge_Payment_Schedule_list( '123', 'TC')}",con1, "##CDOR_HEDGE_PAYMENT_SCHEDULE", "CDOR_ALIM_HEDGE_HEDGE_PAYMENT_SCHEDULE");
		   	
		   	//db.CallProc("{call dbo.CDOR_EXTRACTION_UPDATE ('83','5','1','2008-01-29','20446',6	,'466471','2008-10-01','2008-10-30','0','1','0','0','test')}",con1);
		   	//db.CallProc("{call dbo.CDOR_EXTRACTION_UPDATE ('349','5','1','2008-02-20','20938',6,'466471','2008-10-01','2008-10-30','0','1','0','0','test')}",con1);
		   	
		   	//db.CreateSeliniumCode("select * from tt",con1);
		   	
	
		   	
		   	//db.CallProc("{call dbo.CDOR_EXTRACTION_HISTORY_INSERT  ('114','1','0','0')}",con1);
		   	//db.CallProc("{call dbo.CDOR_EXTRACTION_HISTORY_UPDATE  ('114','1','0','0')}",con1);
		   	
		   	
		   	//CDOR_EXTRACTION_HISTORY_INSERT @extraction_id, @IS_WEEKLY, @IS_MONTHLY, @IS_QUATERLY
		   	
		   	
		   	 
		   	//db.CreateProcUpdate("{call dbo.Issue_SF_list ( '15587')}",con1, "##CDOR_ISSUE_SF_SCHEDULE", "CDOR_ALIM_ISSUE_SF_SCHEDULE");
		   	//db.CreateProc("{call CDOSUITE..Cash_Transaction_search ( '466471',null,null,'',null,'ALL',null,null,'TRD','A','2001-01-01','2070-01-01 23:59:59:999','TIP',null,'TRD')}",con1,"##CDOR_EXPECTED_PAYMENT_RPT","CDOR_ALIM_EXPECTED_PAYMENT_RPT"); 
		   	//db.CreateProcUpdate("{call Cash_Transaction_search ( '466471',null,null,'35',null,'ALL',null,null,'TRD','A','2001-01-01','2070-01-01 23:59:59:999','TIP',null,'TRD')}",con1,"##CDOR_EXPECTED_PAYMENT_REPORT","CDOR_ALIM_EXPECTED_PAYMENT_REPORT");
		   	  //db.CreateProc("{call PS_Deal_Data_get        ( '20010' , 'PDCH'                                                                             )}",con1, "##CDOR_DEAL_PD_ACCOUNT_BALANCE", "CDOR_ALIM_DEAL_PD_ACCOUNT_BALANCE");
		   	//db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'PDCH')}",con1, "##CDOR_DEAL_PD_ACCOUNT_BALANCE", "CDOR_ALIM_DEAL_PD_ACCOUNT_BALANCE");
		   	//db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'CASH')}",con1, "##CDOR_DEAL_CASH_ACCOUNT_BALANCE", "CDOR_ALIM_DEAL_CASH_ACCOUNT_BALANCE");
		   	//db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'EI')}",con1, "##CDOR_DEAL_ELIGIBLE_INVESTMENT", "CDOR_ALIM_ELIGIBLE_INVESTMENT");
		   	//db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'PDEI')}",con1, "##CDOR_DEAL_PD_ELIGIBLE_INVESTMENT", "CDOR_ALIM_DEAL_PD_ELIGIBLE_INVESTMENT");
		   	//db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'CPEX')}",con1, "##CDOR_DEAL_CP_EXPENSE", "CDOR_ALIM_DEAL_CP_EXPENSE");
		   	//db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'CPNC')}",con1, "##CDOR_DEAL_CP_NET_COLLECTION", "CDOR_ALIM_DEAL_CP_NET_COLLECTION");
		   	//db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'CE')}",con1, "##CDOR_DEAL_CASH_EQUIVALENT", "CDOR_ALIM_DEAL_CASH_EQUIVALENT");
		   	//db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'PJST')}",con1, "##CDOR_DEAL_PROJECTION_SETTING", "CDOR_ALIM_DEAL_PROJECTION_SETTING");
		   	//db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'PJPD')}",con1, "##CDOR_DEAL_PAYMENT_SCHEDULE", "CDOR_ALIM_DEAL_PAYMENT_SCHEDULE");
		   	//db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'RIST')}",con1, "##CDOR_REINVESTMENT_SETTING", "CDOR_ALIM_REINVESTMENT_SETTING");
		   	//db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'RIIR')}",con1, "##CDOR_REINVESTMENT_INTEREST_RATE", "CDOR_ALIM_REINVESTMENT_INTEREST_RATE");
		   	//db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'CER')}",con1, "##CDOR_CURRENCY_EXCHANGE_RATE", "CDOR_ALIM_CURRENCY_EXCHANGE_RATE");
		   	//db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'ICAD')}",con1, "##CDOR_IC_TEST_ADJUSTMENT", "CDOR_ALIM_IC_TEST_ADJUSTMENT");
		    //db.CreateProcUpdate("{call PS_Deal_Data_get ( '20010' , 'OCAD')}",con1, "##CDOR_OVERCOLLATERALIZATION_TEST_ADJUSTMENT", "CDOR_ALIM_OVERCOLLATERALIZATION_TEST_ADJUSTMENT");
		   	//db.CreateProcUpdate("{call PS_portfolio_snapshot_list('35', 'MR', null, null, null, null, null, null)}",con1, "##CDOR_OVERCOLLATERALIZATION_TEST_ADJUSTMENT", "CDOR_ALIM_OVERCOLLATERALIZATION_TEST_ADJUSTMENT");
		   // country list tttti	db.CreateProc("{call cdosys_lookup_code_list ( '17' )}",con1, "##CDOR_COUNTRY_CODE", "CDOR_ALIM_COUNTRY_CODE");
		   	//dbo.cdosys_Lookup_code_list @lookup_type_id=17
		     
		   	//db.CreateProc("{call cdosys_lookup_code_list ( '237' )}",con1, "##CDOR_BANK_ROLE_TYPE", "CDOR_ALIM_BANK_ROLE_TYPE");
		   	//db.CreateProcUpdate("{call cdosys_lookup_code_list ( '237' )}",con1, "##CDOR_BANK_ROLE_TYPE", "CDOR_ALIM_BANK_ROLE_TYPE");
		   	// db.CreateProc("{call cdosys_lookup_code_list ( '112' )}",con1, "##CDOR_REGION_TYPE", "CDOR_ALIM_REGION_TYPE");
		   	//db.CreateProcUpdate("{call cdosys_lookup_code_list ( '112' )}",con1, "##CDOR_REGION_CODE", "CDOR_ALIM_REGION_CODE");
		   	//db.CreateProc("{call cdosys_lookup_code_list ( '238' )}",con1, "##CDOR_ABS_SERVICER_TYPE", "CDOR_ALIM_ABS_SERVICER_TYPE");
		   	//db.CreateProc("{call cdosys_lookup_code_list ( '2' )}",con1, "##CDOR_ABS_SPEC_TYPE", "CDOR_ALIM_ABS_SPEC_TYPE");
		   	//db.CreateProcUpdate("{call cdosys_lookup_code_list ( '2' )}",con1, "##CDOR_ABS_SPEC_TYPE", "CDOR_ALIM_ABS_SPEC_TYPE");
		   	//db.CreateProc("{call cdosys_lookup_code_list ( '3' )}",con1, "##CDOR_ABS_TYPE", "CDOR_ALIM_ABS_TYPE");
		   	//db.CreateProc("{call cdosys_lookup_code_list ( '239' )}",con1, "##CDOR_ASSET_TYPE_MOODYS", "CDOR_ALIM_ASSET_TYPE_MOODYS");
		   	//db.CreateProcUpdate("{call cdobiz_Industry_list ( 'FA' )}",con1, "##CDOR_RATING", "CDOR_ALIM_RATING");
		   	//db.CreateProcUpdate("{call test()}",con1, "##CDOR_CDOBIZ_RATING", "CDOR_ALIM_CDOBIZ_RATING");
		   		 
		   	//EXEC cdobiz_Industry_list FA' 

		   	//db.CreateProc("{call test ()}",con1, "##CDOR_FACILITY_TRADE", "CDOR_ALIM_FACILITY_TRADE");
		   	//db.CreateProcUpdate("{call cdosys_lookup_code_list ( '239' )}",con1, "##CDOR_ALIM_SERVICER_RATING_ASSET_TYPE", "CDOR_ALIM_SERVICER_RATING_ASSET_TYPE");
		   	//db.CreateProcUpdate("{call cdosys_lookup_code_list ( '258' )}",con1, "##CDOR_SYSTEM_ENTITY_TYPE", "CDOR_ALIM_SYSTEM_ENTITY_TYPE");
		   	String[] Tabls = { "##CDOR_DEAL_CASH_ACCOUNT_BALANCE" ,  //Ok
		   			"##CDOR_DEAL_CASH_EQUIVALENT" ,  //OK
		   			"##CDOR_DEAL_CP_EXPENSE", //OK
		   			"##CDOR_DEAL_CP_NET_COLLECTION", //OK
		   			"##CDOR_DEAL_CURRENCY_EXCHANGE_RATE", //KO
		   			"##CDOR_DEAL_ELIGIBLE_INVESTMENT", //OK
		   			"##CDOR_DEAL_IC_TEST_ADJUSTMENT",  //KO
		   			"##CDOR_DEAL_OC_TEST_ADJUSTMENT", //KO
		   			"##CDOR_DEAL_PAYMENT_SCHEDULE", //KO
		   			"##CDOR_DEAL_PD_ACCOUNT_BALANCE", //OK
		   			"##CDOR_DEAL_PD_ELIGIBLE_INVESTMENT", //OK
		   			"##CDOR_DEAL_PROJECTION_SETTING",  //KO
		   			"##CDOR_DEAL_REINVESTMENT_INTEREST_RATE", //KO
		   			"##CDOR_DEAL_REINVESTMENT_SETTING"};  //KO
		   	
		   	String[] Tabls2 = { "CDOR_DEAL_CASH_ACCOUNT_BALANCE" ,  //Ok
		   			"CDOR_DEAL_CASH_EQUIVALENT" ,  //OK
		   			"CDOR_DEAL_CP_EXPENSE", //OK
		   			"CDOR_DEAL_CP_NET_COLLECTION", //OK
		   			"CDOR_DEAL_ELIGIBLE_INVESTMENT", //OK
		   			"CDOR_DEAL_PD_ACCOUNT_BALANCE", //OK
		   			"CDOR_DEAL_PD_ELIGIBLE_INVESTMENT" //OK		   			
		   			}; 
		   //	db.Createdforeignkey("Test" , con1, Tabls,Tabls2 , "CDOR_DEAL_CASH_Init_Base");
		   	//db.CreateProcBase("Test" , con1, Tabls , "CDOR_DEAL_CASH_Init_Base");
		   	//db.CreateProc("{call PS_Deal_Data_get ( '20010' , 'PDCH')}",con1, "##CDOR_DEAL_PD_ACCOUNT_BALANCE", "CDOR_ALIM_DEAL_PD_ACCOUNT_BALANCE");
		   	//db.CreateProc("{call PS_Deal_Data_get ( '20010' , 'PDEI')}",con1, "##CDOR_DEAL_PD_ELIGIBLE_INVESTMENT", "CDOR_ALIM_DEAL_PD_ELIGIBLE_INVESTMENT");
		   	//db.CreateProc("{call PS_Deal_Data_get ( '20010' , 'CPEX')}",con1, "##CDOR_DEAL_CP_EXPENSE", "CDOR_ALIM_DEAL_CP_EXPENSE");
		   	//db.CreateProc("{call PS_Deal_Data_get ( '20010' , 'CPNC')}",con1, "##CDOR_DEAL_CP_NET_COLLECTION", "CDOR_ALIM_DEAL_CP_NET_COLLECTION");
		   	//db.CreateProc("{call PS_Deal_Data_get ( '20010' , 'CE')}",con1, "##CDOR_DEAL_CASH_EQUIVALENT", "CDOR_ALIM_DEAL_CASH_EQUIVALENT");
		   	//db.CreateProc("{call PS_Deal_Data_get ( '20010' , 'PJST')}",con1, "##CDOR_DEAL_PROJECTION_SETTING", "CDOR_ALIM_DEAL_PROJECTION_SETTING");
		   	//db.CreateProc("{call PS_Deal_Data_get ( '20010' , 'PJPD')}",con1, "##CDOR_DEAL_PAYMENT_SCHEDULE", "CDOR_ALIM_DEAL_PAYMENT_SCHEDULE");
		   	//db.CreateProc("{call PS_Deal_Data_get ( '20010' , 'RIST')}",con1, "##CDOR_REINVESTMENT_SETTING", "CDOR_ALIM_REINVESTMENT_SETTING");
		   	//db.CreateProc("{call PS_Deal_Data_get ( '20010' , 'RIIR')}",con1, "##CDOR_REINVESTMENT_INTEREST_RATE", "CDOR_ALIM_REINVESTMENT_INTEREST_RATE");
		   	//db.CreateProc("{call PS_Deal_Data_get ( '20010' , 'CER')}",con1, "##CDOR_CURRENCY_EXCHANGE_RATE", "CDOR_ALIM_CURRENCY_EXCHANGE_RATE");
		   	//db.CreateProc("{call PS_Deal_Data_get ( '20010' , 'ICAD')}",con1, "##CDOR_IC_TEST_ADJUSTMENT", "CDOR_ALIM_IC_TEST_ADJUSTMENT");
		   	//db.CreateProc("{call PS_Deal_Data_get ( '20010' , 'OCAD')}",con1, "##CDOR_OVERCOLLATERALIZATION_TEST_ADJUSTMENT", "CDOR_ALIM_OVERCOLLATERALIZATION_TEST_ADJUSTMENT");
		   	//db.CreateProc("{call dbo.Cash_Account_Activity_search('364657',  169, 'STLD', NULL,  NULL,  '2007-11-20', '2007-12-20', 'TRD')}",con1);
		   	//db.CreateProc("{call PS_portfolio_snapshot_list('35', 'MR', null, null, null, null, null, null)}",con1, "CDOR_PS_List", "CDOR_PS_List");
		   	
		   	//db.disconnect();
	   }	
}
	   

