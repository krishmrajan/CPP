PROCEDURE DBA_GENAUDITTRIGGER(PV_NOM_TABLE_I IN VARCHAR2)
IS
RECTYPE CONSTANT VARCHAR2(100) := PV_NOM_TABLE_I || '%ROWTYPE';
COLS DBMS_SQL.varchar2s;
LV_OLDNEW VARCHAR2(20) ;
OLDNEW    VARCHAR2(20);

PROCEDURE GENPROC(PV_OLDNEW_I IN VARCHAR2 ,
                  PV_TYPE_I   IN VARCHAR2 )
IS

BEGIN
   IF PV_TYPE_I  = 'COPIE'
   THEN
      BEGIN
          IF  PV_OLDNEW_I ='ANCIEN'
          THEN
             OLDNEW :='OLD';
          ELSIF PV_OLDNEW_I ='NOUVEAU'
          THEN
             OLDNEW :='NEW';
          END IF;
          LV_OLDNEW := PV_OLDNEW_I;
          DBMS_OUTPUT.PUT_LINE ('-------------------------------------------------------------------------------');
          DBMS_OUTPUT.put_line( 'FUNCTION COPIE_' || LV_OLDNEW || ' RETURN ' || recType || ' IS lV_RETOUR ' || recType || '; ' );
          DBMS_OUTPUT.PUT_LINE ('-------------------------------------------------------------------------------');
          DBMS_OUTPUT.put_line('BEGIN');
          FOR indx IN 1 .. COLS.COUNT
          LOOP
             DBMS_OUTPUT.PUT_LINE(RPAD('   LV_RETOUR.' || COLS(INDX) ,50) || RPAD( ':=  :' || OLDNEW || '.' || COLS(INDX) || ';' ,70));
          END LOOP;
          DBMS_OUTPUT.PUT_LINE('   RETURN lV_RETOUR;');
          DBMS_OUTPUT.PUT_LINE('END COPIE_' || LV_OLDNEW || ';');
      END ;
   END IF;

   IF PV_TYPE_I  = 'INSERT'
   THEN
      BEGIN
         DBMS_OUTPUT.PUT_LINE ('-------------------------------------------------------------------------------');
         DBMS_OUTPUT.PUT_LINE( 'FUNCTION INSERT_' || LV_OLDNEW || '( PV_ROWTYPE_I '  ||  recType || ')  RETURN NUMBER IS    ');
         DBMS_OUTPUT.PUT_LINE ('-------------------------------------------------------------------------------');
         DBMS_OUTPUT.PUT_LINE('BEGIN');
         DBMS_OUTPUT.PUT_LINE('   INSERT INTO AUDIT_' || PV_NOM_TABLE_I || ' ( ');
         FOR indx IN 1 .. COLS.COUNT
         LOOP
         DBMS_OUTPUT.PUT_LINE('           ' || RPAD(COLS(INDX) ,50) || ',' );
         END LOOP;
         DBMS_OUTPUT.PUT_LINE('           ' || RPAD('AUDIT_DATE',50) || ',');
         DBMS_OUTPUT.PUT_LINE('           ' || RPAD('TYPE_CHANGEMENT',50) || ',');
         DBMS_OUTPUT.PUT_LINE('           ' || RPAD('UTILISATEUR_CONNEXION )',50) );
         DBMS_OUTPUT.PUT_LINE('   VALUES (');
         FOR indx IN 1 .. cols.COUNT
         LOOP
            DBMS_OUTPUT.put_line(RPAD('           PV_ROWTYPE_I.' || COLS(INDX) ,60) || ',');
         END LOOP;
         DBMS_OUTPUT.PUT_LINE('           ' ||RPAD('SYSTIMESTAMP' , 49) || ',' );
         DBMS_OUTPUT.PUT_LINE('           ' ||RPAD('LV_TYPE_CHANGEMENT' , 49) || ',' );
         DBMS_OUTPUT.PUT_LINE('           ' ||RPAD('LV_LOG_INFO);' ,49));
         --    DBMS_OUTPUT.PUT_LINE('              COMMIT;');
         DBMS_OUTPUT.put_line('   RETURN 0;');
         DBMS_OUTPUT.put_line('END INSERT_' || LV_OLDNEW || ';');
      END;
   END IF;
END GENPROC;
BEGIN
    SELECT UPPER (COLUMN_NAME) COLUMN_NAME BULK COLLECT INTO COLS FROM ALL_TAB_COLUMNS WHERE TABLE_NAME = UPPER(PV_NOM_TABLE_I) AND OWNER = UPPER(OWNER) ORDER BY COLUMN_ID;
    DBMS_OUTPUT.PUT_LINE('CREATE OR REPLACE TRIGGER TRG_' || PV_NOM_TABLE_I || '_ARIUD');
    DBMS_OUTPUT.PUT_LINE('AFTER INSERT OR UPDATE OR DELETE');
    DBMS_OUTPUT.PUT_LINE('ON ' || PV_NOM_TABLE_I  );
    DBMS_OUTPUT.PUT_LINE('FOR EACH ROW');
    DBMS_OUTPUT.PUT_LINE('DECLARE');
    --DBMS_OUTPUT.PUT_LINE(' PRAGMA AUTONOMOUS_TRANSACTION;');
    DBMS_OUTPUT.PUT_LINE ('-------------------------------------------------------------------------------');
    DBMS_OUTPUT.PUT_LINE ('-- TRIGGER              :  TRG_' || PV_NOM_TABLE_I || '_ARIUD'                                    );
    DBMS_OUTPUT.PUT_LINE ('--'                                                                             );
    DBMS_OUTPUT.PUT_LINE ('--'                                                                             );
    DBMS_OUTPUT.PUT_LINE ('-- DESCRIPTION          :');
    DBMS_OUTPUT.PUT_LINE ('--                      Audit trigger on table ' || PV_NOM_TABLE_I || '  AFTER INSERT/UPDATE/DELETE');
    DBMS_OUTPUT.PUT_LINE ('--');
    DBMS_OUTPUT.PUT_LINE ('--');
    DBMS_OUTPUT.PUT_LINE ('-- ERREURS              :');
    DBMS_OUTPUT.PUT_LINE ('--');
    DBMS_OUTPUT.PUT_LINE ('-- REVISIONS  : DATE         PAR         MOTIF');
    DBMS_OUTPUT.PUT_LINE ('--              ----------- ----------- ---------------------------------------');
    DBMS_OUTPUT.PUT_LINE ('--              JJ/MM/AAAA               CREATION');
    DBMS_OUTPUT.PUT_LINE ('-------------------------------------------------------------------------------');
    DBMS_OUTPUT.PUT_LINE ('');
    DBMS_OUTPUT.PUT_LINE ('-------------------------------------------------------------------------------');
    DBMS_OUTPUT.PUT_LINE ('--                          VARIABLES GESTION ERREUR                         --');
    DBMS_OUTPUT.PUT_LINE ('-------------------------------------------------------------------------------');
    DBMS_OUTPUT.PUT_LINE('LV_TYPE_CHANGEMENT              VARCHAR2(1)                                   ;');
--    DBMS_OUTPUT.PUT_LINE('LV_UTILISATEUR_CONNECTION       VARCHAR2(50)                                  ;');
    DBMS_OUTPUT.PUT_LINE('LIGNE_ANCIEN                    '  ||PV_NOM_TABLE_I || '%ROWTYPE              ;');
    DBMS_OUTPUT.PUT_LINE('LIGNE_NOUVEAU                   '  ||PV_NOM_TABLE_I || '%ROWTYPE              ;');
    DBMS_OUTPUT.PUT_LINE('LV_LOG_INFO                     VARCHAR2(200)                                 ;');
    DBMS_OUTPUT.PUT_LINE('LV_RETURN                       NUMBER                                        ;');
    DBMS_OUTPUT.PUT_LINE ('-------------------------------------------------------------------------------');
    GENPROC('ANCIEN' , 'COPIE');
    GENPROC('NOUVEAU', 'COPIE');
    GENPROC('NOUVEAU', 'INSERT');
    DBMS_OUTPUT.PUT_LINE('BEGIN');
    DBMS_OUTPUT.PUT_LINE('   SELECT ' ||CHR(39)||'Host <'||CHR(39) ||CHR(124) ||CHR(124)||'SYS_CONTEXT('||CHR(39) ||'USERENV'||CHR(39)||','||CHR(39)||'HOST'        ||CHR(39)||' ) '||CHR(124)||CHR(124)
                                      ||CHR(39)||'> OS_USER < '||CHR(39) ||CHR(124) ||CHR(124)||'SYS_CONTEXT('||CHR(39) ||'USERENV'||CHR(39)||','||CHR(39)||'OS_USER'     ||CHR(39)||' ) '||CHR(124)||CHR(124)
                                      ||CHR(39)||'> LOGIN   < '||CHR(39) ||CHR(124) ||CHR(124)||'SYS_CONTEXT('||CHR(39) ||'USERENV'||CHR(39)||','||CHR(39)||'SESSION_USER'||CHR(39)||' ) '||CHR(124)||CHR(124)
                                      ||CHR(39)||'>'||CHR(39) ||' INTO LV_LOG_INFO  FROM  dual;');
    DBMS_OUTPUT.PUT_LINE('   LIGNE_ANCIEN  := COPIE_ANCIEN();');
    DBMS_OUTPUT.PUT_LINE('   LIGNE_NOUVEAU := COPIE_NOUVEAU();');
    DBMS_OUTPUT.PUT_LINE('   IF INSERTING THEN');
    DBMS_OUTPUT.PUT_LINE('      LV_TYPE_CHANGEMENT := '||CHR(39) || 'I' ||CHR(39)||';');
    DBMS_OUTPUT.PUT_LINE('      LV_RETURN := INSERT_NOUVEAU(LIGNE_NOUVEAU);');
    DBMS_OUTPUT.PUT_LINE('   ELSIF UPDATING THEN');
    DBMS_OUTPUT.PUT_LINE('        LV_TYPE_CHANGEMENT := '||CHR(39) ||'U' || CHR(39)||';');
    DBMS_OUTPUT.PUT_LINE('      LV_RETURN := INSERT_NOUVEAU(LIGNE_NOUVEAU);');
    DBMS_OUTPUT.PUT_LINE('   ELSIF DELETING THEN');
    DBMS_OUTPUT.PUT_LINE('        LV_TYPE_CHANGEMENT := '|| CHR(39)||'D'||CHR(39)||';');
    DBMS_OUTPUT.PUT_LINE('      LV_RETURN := INSERT_NOUVEAU(LIGNE_ANCIEN);');
    DBMS_OUTPUT.PUT_LINE('   END IF;');
    DBMS_OUTPUT.PUT_LINE('END;');
END DBA_GENAUDITTRIGGER;
