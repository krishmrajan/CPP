PROCEDURE DBA_GEN_AUDITTABLEDDL(PV_NOM_TABLE_I      LISTE_TABLE_AUDIT.NOM_TABLE%TYPE ,
                                PV_EXECUTE_I        NUMBER     DEFAULT 0
                                     )
IS
-------------------------------------------------------------------------------
-- PROCEDURE            :  AGW.DBA_GEN_AUDITTABLEDDL
--
-- PARAMETRES DENTREE  :
--
-- PARAMETRES DE SORTIE :
--
-- RETOUR               :  1  --> OK
--                         0  --> KO
--
-- DESCRIPTION          :
--
--
--
-- ERREURS              :
--
-- REVISIONS  : DATE         PAR         MOTIF
--              ----------- ----------- ---------------------------------------
--              JJ/MM/AAAA               CREATION
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
--                          VARIABLES GESTION ERREUR                         --
-------------------------------------------------------------------------------
LV_APP_ERROR           NUMBER         := 0                                    ;
LV_DESC_APP_ERROR      VARCHAR2(1024) := NULL                                 ;
LV_RAISE_ERROR         EXCEPTION                                              ;
-------------------------------------------------------------------------------
--                            VARIABLES PROCEDURE                            --
-------------------------------------------------------------------------------
LV_NOM_TABLE          VARCHAR2(60)                                            ;
LV_SQL                VARCHAR2(2000)                                          ;
-------------------------------------------------------------------------------

BEGIN
   BEGIN
      LV_NOM_TABLE  := pkg_dba_outils.f_trimvariable(pv_str => 'AUDIT_'|| PV_NOM_TABLE_I );
      LV_SQL := ' CREATE TABLE ' || LV_NOM_TABLE || '
      AS  SELECT LPAD('||chr(39)||' '||chr(39)||',1,'||chr(39)||' '||chr(39)|| ')  AS TYPE_CHANGEMENT   ,
      LOCALTIMESTAMP AS AUDIT_DATE                   ,
      LPAD('||chr(39)||' '||chr(39)|| ', 100, '||chr(39)||' ' ||chr(39)|| ') as UTILISATEUR_CONNEXION ,'
      || PV_NOM_TABLE_I ||'.* FROM ' || PV_NOM_TABLE_I || ' WHERE ROWNUM < 1';

       IF PV_EXECUTE_I = 1
       THEN
          BEGIN
             EXECUTE IMMEDIATE lV_SQL;
          END;
       END IF ;
       DBMS_OUTPUT.PUT_LINE(LV_SQL || '; ' || chr(10)|| '/' );
       LV_SQL :='ALTER TABLE '||LV_NOM_TABLE || ' MODIFY ( TYPE_CHANGEMENT NOT NULL , AUDIT_DATE NOT NULL , UTILISATEUR_CONNEXION NOT NULL ) ';
       DBMS_OUTPUT.PUT_LINE(LV_SQL || ' ;' || chr(10) || '/');
       IF PV_EXECUTE_I  = 1
       THEN
          BEGIN
             EXECUTE IMMEDIATE LV_SQL ;
          END;
       END IF;
   END ;
--====================================
-- DEBUT TRAITEMENT
--====================================

--====================================
-- SORTIE NORMALE
--====================================
<<SORTIE_OK>>
   NULL;


   IF LV_DESC_APP_ERROR IS NULL  THEN
      LV_DESC_APP_ERROR := TO_CHAR(SQLCODE)||' - '||SQLERRM||' PROC.= AGW.DBA_GEN_AUDITTABLEDDL';
   END IF;

   -- ------------------------------------------------------- --
   -- VALUES THAT APP_ERROR CAN TAKE:                         --
   --              0 - OK                                     --
   --              1 - ORACLE SQL ERROR NOT PREDICTED         --
   --              9 - FUNCTIONAL ERROR                       --
   --                                                         --
   -- THE FIELD DESC_APP_ERROR SHOULD CONTAIN THE MESSAGE TO  --
   -- PASS TO THE JAVA LAYER, THIS MESSAGE SHOULD BE BUILT    --
   -- AT RAISE TIME.                                          --
   -- FOR THE CASES WHERE THE LV_DESC_APP_ERROR MESSAGE       --
   -- HAS NOT BEEN BUILT IN THE PREVIOUS EXCEPTION BLOCKS     --
   -- IT SHOULD BE BUILT IN THIS EXCEPTION BLOCK              --
   -- IF THE "LV_DESC_APP_ERROR IS NULL" TEST IS TRUE         --
   -- ------------------------------------------------------- --
      IF LV_APP_ERROR = 9 THEN
         RAISE_APPLICATION_ERROR(-20998,'|'||LV_DESC_APP_ERROR||'|');
      ELSE
           RAISE_APPLICATION_ERROR(-20999,'ERROR MESSAGE: '||LV_DESC_APP_ERROR);
      END IF;

END DBA_GEN_AUDITTABLEDDL;
