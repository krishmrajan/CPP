*---------------------------------------------------------------------------------------*/
/*   F_TRIMVARIABLE                                                                      */
/*---------------------------------------------------------------------------------------*/
FUNCTION F_TRIMVARIABLE (PV_STR IN VARCHAR2) RETURN VARCHAR2 AS
-------------------------------------------------------------------------------
-- FUNCTION                : F_TRIMVARIABLE
--
-- INPUT PARAMETERS        : STRING
--
--
-- OUTPUT PARAMETERS       : STRING
--
-- DESCRIPTION             : THIS FUNCTION REMOVES THE VOWELS IN THE GIVEN STRING IF
--                           LENGTH OF THE GIVEN STRING IS GREATER THEN 30 CHARACTERS.OTHERWISE
--                           IT WILL RETURN THE GIVEN STRING AS IT IS.
--
-- EXAMPLES                  VALID INPUT STRING:
--                           INPUT        : PV_APPOINTMENT_ACTIONNAIRE_IO
--                           OUTPUT       : PV_APPOINTMENT_ACTIONNAIRE_IO
--                           INPUT        : PV_APPOINTMENT_EMETTEUR_ACTIONNAIRE_IO
--                           OUTPUT       : PV_APPNTMNT_EMTTR_ACTNNR_IO
--                           INPUT        : PV_APPOINTMENT_VALUER_FERMER_ANNULAR_EMETTEUR_ACTIONNAIRE_I
--                           OUTPUT       : PV_APPNTMNT_VLR_FRMR_ANNLR_EMT
--
-- ERROR CASES               NULL (RETURNS NULL )
--                           VOWELS CAN'T BE REMOVED NEXT TO THE CHARACTERS(~,!,@,#,$,%,^,&,*,(,),,,-,+,<,>,?,/,",;,',:,{,},[,],||,\)
-- ERRORS                  :
--
-- REVISIONS    DATE        BY          MOTIF
--              ----------- ----------- ---------------------------------------
--              26/03/2010  VINOTH R    CREATION
--              06/04/2010  K.RAJAN     Modification to include the prefix and suffix
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
--                          VARIABLE ERROR MANAGEMENT                        --
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
--                            VARIABLES FUNCTION                             --
-------------------------------------------------------------------------------
 LV_STR                       VARCHAR2(250)                                   ;
 LV_SUFFIX                    VARCHAR2(250)                                   ;
 LV_PREFIX                    VARCHAR2(250)                                   ;
 LV_PREFIXPOS                 NUMBER                                          ;
 LV_SUFFIXPOS                 NUMBER                                          ;
 LV_POS                       NUMBER                                          ;
 LV_WORD                      VARCHAR2(250)                                   ;
 LV_LENGTH                    NUMBER                                          ;
 -----------------------------------------------------------------------------
 BEGIN
   IF LENGTH(PV_STR) > 30
   THEN


      LV_PREFIXPOS := INSTR(PV_STR, '_', 1);                       --GET PRERIX POS
      LV_SUFFIXPOS := INSTR(PV_STR, '_', -1);                      --GET SUFFIX POS


      LV_PREFIX    := SUBSTR(PV_STR, 1, LV_PREFIXPOS );            --GET PREFIX

      LV_SUFFIX    := SUBSTR(PV_STR, LV_SUFFIXPOS ,LENGTH(PV_STR) - (LV_SUFFIXPOS - 1) ) ; -- GET SUFFIX


      LV_STR       := SUBSTR(PV_STR, LV_PREFIXPOS + 1    , LENGTH(PV_STR) -  (LV_PREFIXPOS + LENGTH(LV_SUFFIX)));      --ACTUAL STRING TO TREAT

      LV_LENGTH    := 30 - LENGTH(LV_PREFIX)+ LENGTH(LV_SUFFIX);

      LV_POS       :=  0 ;

      LV_POS       := INSTR(LV_STR,'_',1);

      WHILE LV_POS   > 0  LOOP --AND LENGTH(LV_STR)  > 30 - LV_LENGTH LOOP

            LV_WORD := SUBSTR(LV_STR,1,LV_POS-1);

            LV_WORD := REGEXP_REPLACE(INITCAP(LV_WORD),'[aieou]','');

            LV_STR  := LV_WORD || SUBSTR(LV_STR , LV_POS   );

            --DBMS_OUTPUT.PUT_LINE(LV_STR || ' Length ' || LENGTH(LV_STR) || ' last position ' || LV_POS);

            LV_POS  := INSTR(LV_STR, '_' , LV_POS+1);

        END LOOP;

      LV_STR := LV_PREFIX || SUBSTR(LV_STR, 1, LV_LENGTH) || LV_SUFFIX ;

   ELSE
      RETURN UPPER(PV_STR);
   END IF;
   RETURN UPPER(LV_STR );
END;



