' Capture Path template revision level 19980704.1778
'                                      major <-|-> minor
'
' The revision level is nominally the date and time of the template used to transform a
' Capture Path from the metaformat into Visual Basic code. The date portion (left of the
' decimal) is actually the major revision number. If this number changes, it indicates
' a change in the interface between the VBA code and the Routing Agent, so the Capture
' Path _must_ be regenerated. The time portion (right of the decimal) is actually the
' minor revision number. If only the minor revision number changes, there has been no
' change to the VBA/Agent interface, and regeneration is optional.

Attribute VB_Name = "PathCapturePath"
Option Explicit
Option Base 1

' Shared constants
Public Const CID_NAME_DONE_FLAG           As String = "Done"
Public Const CID_NAME_ERROR_FLAG          As String = "Error"
Public Const CID_NAME_ABORT_FLAG          As String = "Abort"

' Shared constants that mirror Routing Agent values.
Public Const fnssCompletedOk              As Integer = 1
Public Const fnssStoppedError             As Integer = 2
Public Const fnssSuspendedMissing         As Integer = 3
Public Const fnssSuspendedTransit         As Integer = 4
Public Const fnssStoppedTrap              As Integer = 5
Public Const fnssStoppedBadBee            As Integer = 6
Public Const fnssNotRerun                 As Integer = 7
Public Const fnssStoppedAbort             As Integer = 8
Public Const fnssSuspendedBeeReject       As Integer = 9
Public Const fnspPermitRun                As Integer = 1
Public Const fnspAbort                    As Integer = 2
Public Const IDS_BMSG_BATCH_COMPLETED     As Long = 57363
Public Const IDS_BMSG_BATCH_ERRORED       As Long = 57364
Public Const IDS_BMSG_BATCH_ABORTED       As Long = 57365
Public Const IDS_BMSG_INVALID_REPOBJECT   As Long = 57366
Public Const IDS_BMSG_NOFIND_REPROOT      As Long = 57367
Public Const IDS_BMSG_INVALID_START       As Long = 57368
Public Const IDS_BMSG_UNAVAILABLE_BEE     As Long = 57374
Public Const IDS_BMSG_BATCH_WITH_PAGES    As Long = 57375
Public Const IDS_PBFROUTINGAGENT_PPG_CAPTION   As Long = 100

' Shared constants that mirror Capture Resources.
Public Const CID_GROUP_CONTROL            As Long = 132
Public Const CID_NAME_STARTING_PHASE      As Long = 6539

' Shared constants that mirror the repository.
Public Const fnemFinish                   As Integer = 0
Public Const fnemAbort                    As Integer = 1
Public Const fnemPause                    As Integer = 2
Public Const fnemResume                   As Integer = 3
Public Const fnemSynchronous              As Integer = 4
Public Const fnemAsynchronous             As Integer = 5
Public Const fnrsBusy                     As Integer = 3
Public Const fnrsDetached                 As Integer = 4

' Public variables.
Public DebugIndent As Integer
Public WorkArray() As Object

' Public variables for language translation.
Public CID_TITLE_BAR                      As String

Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)


' *** Assumptions:
'   o Somebody else calls the RepObj.Save method, when appropriate.
'   o This CapturePath does no granularity transformation.  It is assumed that
'     the Worker Bee base class does that.
'
' Note that there are numerous references to methods and properties that begin
' with "Agent". These belong to a special object that is created by the Routing
' Agent and exposed to the VBA environment. It is through the methods and
' properties of this object that this VBA CapturePath communicates with the larger
' Panama environment. If you were to devise a VB or VBA program that did not run
' in the Routing Agent's Panama environment, you would have to replace every
' Agent... with something else.


' This is the main body and initial entry point for the Routing CapturePath.
'
' The following Worker Bees are required for execution of this Routing CapturePath:
'   Panagon Capture Commit
'     shorthand = Commit    
'     GUID = {26B5001C-8500-11D0-A60A-0000C0A01A47}
'     internal designation = Commit
'     tool tip = Commit the selected folders or batches.
'     ProgId = FileNET.Capture.Component.Commit
'     BES: PhaseName = 'Commit' PhaseNumber = 8
'   Panagon Capture Assembly
'     shorthand = Assembly    
'     GUID = {697074E3-62A6-11D0-A4DA-00A024385D4F}
'     internal designation = Assembly
'     tool tip = Assembles scanned pages into batches and documents.
'     ProgId = FileNET.Capture.Component.Assembly
'     BES: PhaseName = 'Assembly' PhaseNumber = 4
'   Panagon Capture Image Verify
'     shorthand = Image Verify    
'     GUID = {74CBE3BA-DE6B-11CF-A8B1-0000C0F384CF}
'     internal designation = Image_Verify
'     tool tip = Verifies the captured image(s)
'     ProgId = FileNET.Capture.Component.ImageVerify
'     BES: PhaseName = 'ImageVerify' PhaseNumber = 2
'   Panagon Capture Event Activator
'     shorthand = Event Activator    
'     GUID = {3BAAAAC3-4756-11D0-B43C-9EAF26000000}
'     internal designation = Event_Activator
'     tool tip = Sets properties based on other properties
'     ProgId = FileNET.Capture.Component.EventMgr
'     BES: PhaseName = 'EventActivator' PhaseNumber = 20
'   Panagon Capture Scan
'     shorthand = Scan    
'     GUID = {9F89EABA-7122-11D3-BF5D-00C04F8CFC3F}
'     internal designation = Scan_P0
'     tool tip = Scan Status Prompt
'     ProgId = FileNET.Capture.Component.Scan
'     BES: PhaseName = 'Scan' PhaseNumber = 1
'   Panagon Capture Index Control
'     shorthand = Index    
'     GUID = {692CF244-27E6-11D0-B426-56263B000000}
'     internal designation = Index
'     tool tip = Displays interface for entering index field values
'     ProgId = FileNET.Capture.Component.Index
'     BES: PhaseName = 'Index' PhaseNumber = 5
'
' Throughout this code, the following convention is used:
'   DoNAMEWorker - label used for GoTo
'   BeeNAME      - object (pointer) for the Bee
' In both cases, we use a prefix because special nodes have names beginning with "_".
'
Sub Main()
    DebugIndent = 0
    PrintDebug("CapturePath starting up.")

    ' Declarations
    Dim Probe As Object
    Dim ErrorMessage As String
    Dim WorkedOK As Boolean
    Dim Junk
    Dim InSide As Integer
    Dim OutAlternativeCode As Variant
    Dim StartingBeeUnique As String
    Dim AttemptingBeeUnique As String
    Dim LastCompleteBeeUnique As String
    Dim WorkItemV As Variant
    Dim WorkItem As Object
    Dim iIndex As Integer
    Dim LastCall As Boolean
    Dim LocalCompletionStatus As Integer

    ' Declare the Worker Bees.
    Dim BeeCommit As Object
    Dim BeeAssembly As Object
    Dim BeeImage_Verify As Object
    Dim BeeEvent_Activator As Object
    Dim BeeScan_P0 As Object
    Dim BeeIndex As Object

    ' Initialization
    LastCompleteBeeUnique = ""
    Call MakeNilWorkArray
    LastCall = False

    ' Load language translation strings.
    CID_TITLE_BAR = AgentGetMessage(IDS_PBFROUTINGAGENT_PPG_CAPTION)
    
    ' Junk = MsgBox("Capture Path execution begins...", 0, CID_TITLE_BAR)
        
    ' Take control of errors.
    On Error GoTo ErrorTrap
           
           
    '=============================================================
    ' Repository and RepObject setup.
    '=============================================================
           
           
    '-------------------------------------------------------------
    ' Declare the document hierarchy.
    '   Folder -> Batch -> Doc -> Side
    ' Note that the actual layout of the repository has no Page objects.
    ' This actually reflects the reality that a document is made up of
    ' images.  Sure, they existed for a while printed two to a piece of
    ' paper.  But that was transitory.  And if any Bee needs that, he
    ' must fake it by accepting Sides and remembering some until the
    ' matching other side appears.
    '-------------------------------------------------------------
    Dim IncomingRepObject As Object
    Dim RepRoot As Object
    Dim ScanRecoverState As String
    
    ' A RepObject will be given to the Routing CapturePath for processing.  Get it
    ' in 'IncomingRepObject', the unit of work that is to be processed.
    ' Then make sure that we were given a good RepObject.
    PrintDebug("Getting IncomingRepObject.")
    DebugIndent = DebugIndent + 1
    Set IncomingRepObject = AgentWorkObject
    If IncomingRepObject Is Nothing Then
        ErrorMessage = GetSubstitutedMessage(IDS_BMSG_INVALID_REPOBJECT, "")
        GoTo DisplayErrorMessage
    End If
    PrintDebug("IncomingRepObject looks good.")
    PrintDebug("IncomingRepObject calls itself '" & IncomingRepObject.Name & "'.")
    DebugIndent = DebugIndent - 1
   
    ' From what we have been given, find the root of the repository.
    Set RepRoot = AgentNavigateRepository(IncomingRepObject, "^", False)
    If RepRoot Is Nothing Then
        ErrorMessage = GetSubstitutedMessage(IDS_BMSG_NOFIND_REPROOT, "")
        GoTo DisplayErrorMessage
    End If
    PrintDebug("Repository looks good.")

    ' Check any content in IncomingRepObject for possible Scan Recovery
    If IncomingRepObject.Contents.Count > 0 Then
        ScanRecoverState = "PagesExist"
    Else
        ScanRecoverState = "None"
    End If

    '-------------------------------------------------------------
    ' This CapturePath does not perform granularity transformation,
    ' so a single work object is routed to all Worker Bees.
    ' That's IncomingRepObject.
    '-------------------------------------------------------------
    
    ' Worker Bees need their work in an array.
    ReDim WorkArray(1)
    Set WorkArray(1) = IncomingRepObject

           
    '=============================================================
    ' Worker Bee setup.
    '=============================================================
           
           
    '-------------------------------------------------------------
    ' Beg, borrow, or steal Worker Bees to use in processing the
    ' work object.
    '-------------------------------------------------------------
    
    PrintDebug("Looking for Worker Bees...")
    DebugIndent = DebugIndent + 1

    ErrorMessage = "Error trap while locating instance of 'FileNET.Capture.Component.Commit' ({26B5001C-8500-11D0-A60A-0000C0A01A47})."
    Set BeeCommit = AgentGetBeeDispatch("Commit_P0")
    If BeeCommit Is Nothing Then
        PrintDebug("'Commit_P0' is not installed on this box.")
    Else
        PrintDebug("Got dispatch pointer for 'Commit_P0'.")
    End If

    ErrorMessage = "Error trap while locating instance of 'FileNET.Capture.Component.Assembly' ({697074E3-62A6-11D0-A4DA-00A024385D4F})."
    Set BeeAssembly = AgentGetBeeDispatch("Assembly_P0")
    If BeeAssembly Is Nothing Then
        PrintDebug("'Assembly_P0' is not installed on this box.")
    Else
        PrintDebug("Got dispatch pointer for 'Assembly_P0'.")
    End If

    ErrorMessage = "Error trap while locating instance of 'FileNET.Capture.Component.ImageVerify' ({74CBE3BA-DE6B-11CF-A8B1-0000C0F384CF})."
    Set BeeImage_Verify = AgentGetBeeDispatch("ImageVerify_P0")
    If BeeImage_Verify Is Nothing Then
        PrintDebug("'ImageVerify_P0' is not installed on this box.")
    Else
        PrintDebug("Got dispatch pointer for 'ImageVerify_P0'.")
    End If

    ErrorMessage = "Error trap while locating instance of 'FileNET.Capture.Component.EventMgr' ({3BAAAAC3-4756-11D0-B43C-9EAF26000000})."
    Set BeeEvent_Activator = AgentGetBeeDispatch("EventMgr_P0")
    If BeeEvent_Activator Is Nothing Then
        PrintDebug("'EventMgr_P0' is not installed on this box.")
    Else
        PrintDebug("Got dispatch pointer for 'EventMgr_P0'.")
    End If

    ErrorMessage = "Error trap while locating instance of 'FileNET.Capture.Component.Scan' ({9F89EABA-7122-11D3-BF5D-00C04F8CFC3F})."
    Set BeeScan_P0 = AgentGetBeeDispatch("Scan_P0")
    If BeeScan_P0 Is Nothing Then
        PrintDebug("'Scan_P0' is not installed on this box.")
    Else
        PrintDebug("Got dispatch pointer for 'Scan_P0'.")
    End If

    ErrorMessage = "Error trap while locating instance of 'FileNET.Capture.Component.Index' ({692CF244-27E6-11D0-B426-56263B000000})."
    Set BeeIndex = AgentGetBeeDispatch("Index_P0")
    If BeeIndex Is Nothing Then
        PrintDebug("'Index_P0' is not installed on this box.")
    Else
        PrintDebug("Got dispatch pointer for 'Index_P0'.")
    End If

    PrintDebug("Done looking for Worker Bees.")
    DebugIndent = DebugIndent - 1
           
           
    '-------------------------------------------------------------
    ' Tell each of the Worker Bees about the Batch. This might well
	' be ignored by most Bees.
    '-------------------------------------------------------------
    
    PrintDebug("Telling all Worker Bees about the Batch...")
    DebugIndent = DebugIndent + 1

    If BeeCommit Is Nothing Then
        PrintDebug("Unavailable 'BeeCommit'.")
    Else
        PrintDebug("Setting 'BeeCommit.CurrentObject'.")
        ErrorMessage = "Panama component error:  Error trap while setting 'Commit.CurrentObject'."
        Set BeeCommit.CurrentObject = IncomingRepObject
        PrintDebug("Setting 'BeeCommit.CurrentObject' returned.")
    End If

    If BeeAssembly Is Nothing Then
        PrintDebug("Unavailable 'BeeAssembly'.")
    Else
        PrintDebug("Setting 'BeeAssembly.CurrentObject'.")
        ErrorMessage = "Panama component error:  Error trap while setting 'Assembly.CurrentObject'."
        Set BeeAssembly.CurrentObject = IncomingRepObject
        PrintDebug("Setting 'BeeAssembly.CurrentObject' returned.")
    End If

    If BeeImage_Verify Is Nothing Then
        PrintDebug("Unavailable 'BeeImage_Verify'.")
    Else
        PrintDebug("Setting 'BeeImage_Verify.CurrentObject'.")
        ErrorMessage = "Panama component error:  Error trap while setting 'Image_Verify.CurrentObject'."
        Set BeeImage_Verify.CurrentObject = IncomingRepObject
        PrintDebug("Setting 'BeeImage_Verify.CurrentObject' returned.")
    End If

    If BeeEvent_Activator Is Nothing Then
        PrintDebug("Unavailable 'BeeEvent_Activator'.")
    Else
        PrintDebug("Setting 'BeeEvent_Activator.CurrentObject'.")
        ErrorMessage = "Panama component error:  Error trap while setting 'Event_Activator.CurrentObject'."
        Set BeeEvent_Activator.CurrentObject = IncomingRepObject
        PrintDebug("Setting 'BeeEvent_Activator.CurrentObject' returned.")
    End If

    If BeeScan_P0 Is Nothing Then
        PrintDebug("Unavailable 'BeeScan_P0'.")
    Else
        PrintDebug("Setting 'BeeScan_P0.CurrentObject'.")
        ErrorMessage = "Panama component error:  Error trap while setting 'Scan_P0.CurrentObject'."
        Set BeeScan_P0.CurrentObject = IncomingRepObject
        PrintDebug("Setting 'BeeScan_P0.CurrentObject' returned.")
    End If

    If BeeIndex Is Nothing Then
        PrintDebug("Unavailable 'BeeIndex'.")
    Else
        PrintDebug("Setting 'BeeIndex.CurrentObject'.")
        ErrorMessage = "Panama component error:  Error trap while setting 'Index.CurrentObject'."
        Set BeeIndex.CurrentObject = IncomingRepObject
        PrintDebug("Setting 'BeeIndex.CurrentObject' returned.")
    End If

    PrintDebug("Done telling all Worker Bees about the Batch.")
    DebugIndent = DebugIndent - 1
               
           
    '-------------------------------------------------------------
    ' Tell each of the Worker Bees that a CapturePath is starting.
    '-------------------------------------------------------------
    
    PrintDebug("Telling all Worker Bees that a CapturePath is starting...")
    DebugIndent = DebugIndent + 1

    If BeeCommit Is Nothing Then
        PrintDebug("Unavailable 'BeeCommit'.")
    Else
        PrintDebug("Calling 'BeeCommit.Reset'.")
        ErrorMessage = "Panama component error:  Error trap while calling 'Commit.Reset'."
        BeeCommit.Reset
        PrintDebug("'BeeCommit.Reset' returned.")
    End If

    If BeeAssembly Is Nothing Then
        PrintDebug("Unavailable 'BeeAssembly'.")
    Else
        PrintDebug("Calling 'BeeAssembly.Reset'.")
        ErrorMessage = "Panama component error:  Error trap while calling 'Assembly.Reset'."
        BeeAssembly.Reset
        PrintDebug("'BeeAssembly.Reset' returned.")
    End If

    If BeeImage_Verify Is Nothing Then
        PrintDebug("Unavailable 'BeeImage_Verify'.")
    Else
        PrintDebug("Calling 'BeeImage_Verify.Reset'.")
        ErrorMessage = "Panama component error:  Error trap while calling 'Image_Verify.Reset'."
        BeeImage_Verify.Reset
        PrintDebug("'BeeImage_Verify.Reset' returned.")
    End If

    If BeeEvent_Activator Is Nothing Then
        PrintDebug("Unavailable 'BeeEvent_Activator'.")
    Else
        PrintDebug("Calling 'BeeEvent_Activator.Reset'.")
        ErrorMessage = "Panama component error:  Error trap while calling 'Event_Activator.Reset'."
        BeeEvent_Activator.Reset
        PrintDebug("'BeeEvent_Activator.Reset' returned.")
    End If

    If BeeScan_P0 Is Nothing Then
        PrintDebug("Unavailable 'BeeScan_P0'.")
    Else
        PrintDebug("Calling 'BeeScan_P0.Reset'.")
        ErrorMessage = "Panama component error:  Error trap while calling 'Scan_P0.Reset'."
        BeeScan_P0.Reset
        PrintDebug("'BeeScan_P0.Reset' returned.")
    End If

    If BeeIndex Is Nothing Then
        PrintDebug("Unavailable 'BeeIndex'.")
    Else
        PrintDebug("Calling 'BeeIndex.Reset'.")
        ErrorMessage = "Panama component error:  Error trap while calling 'Index.Reset'."
        BeeIndex.Reset
        PrintDebug("'BeeIndex.Reset' returned.")
    End If

    PrintDebug("Done telling all Worker Bees that a CapturePath is starting.")
    DebugIndent = DebugIndent - 1


    '=============================================================
    ' The following code allows a Batch Flow CapturePath to be started
    ' somewhere other than the beginning.  This facilitates running
    ' _part_ of a Batch Flow CapturePath on one machine and the rest
    ' elsewhere.
    '=============================================================

    PrintDebug("Determining StartingBeeUnique.")
    DebugIndent = DebugIndent + 1
    StartingBeeUnique = GetAttribute(IncomingRepObject, CID_GROUP_CONTROL, CID_NAME_STARTING_PHASE)
    If StartingBeeUnique = "" Then
        StartingBeeUnique = "_Begin"
    End If
    PrintDebug("StartingBeeUnique is '" & StartingBeeUnique & "'.")
    DebugIndent = DebugIndent - 1

    If StartingBeeUnique = CID_NAME_DONE_FLAG Then
        PrintDebug("This Batch has already completed successfully.")
        ErrorMessage = GetSubstitutedMessage(IDS_BMSG_BATCH_COMPLETED, IncomingRepObject.name)
        Junk = MsgBox(ErrorMessage, vbYesNo + vbQuestion + vbSystemModal, CID_TITLE_BAR)
        If Junk = vbNo Then
            PrintDebug("Operator rejects rerun.")
            LastCompleteBeeUnique = ""
            AttemptingBeeUnique = ""
            LocalCompletionStatus = fnssNotRerun
            GoTo FinishedOK
        End If
        PrintDebug("Operator wants rerun.")
        StartingBeeUnique = "_Begin"
    End If

    If StartingBeeUnique = CID_NAME_ERROR_FLAG Then
        PrintDebug("This Batch has already stopped with an error.")
        ErrorMessage = GetSubstitutedMessage(IDS_BMSG_BATCH_ERRORED, IncomingRepObject.name)
        Junk = MsgBox(ErrorMessage, vbYesNo + vbQuestion + vbSystemModal, CID_TITLE_BAR)
        If Junk = vbNo Then
            PrintDebug("Operator rejects rerun.")
            LastCompleteBeeUnique = ""
            AttemptingBeeUnique = ""
            LocalCompletionStatus = fnssNotRerun
            GoTo FinishedOK
        End If
        PrintDebug("Operator wants rerun.")
        StartingBeeUnique = "_Begin"
        ClearBatch IncomingRepObject
    End If

    If StartingBeeUnique = CID_NAME_ABORT_FLAG Then
        PrintDebug("This Batch has already aborted.")
        ErrorMessage = GetSubstitutedMessage(IDS_BMSG_BATCH_ABORTED, IncomingRepObject.name)
        Junk = MsgBox(ErrorMessage, vbYesNo + vbQuestion + vbSystemModal, CID_TITLE_BAR)
        If Junk = vbNo Then
            PrintDebug("Operator rejects rerun.")
            LastCompleteBeeUnique = ""
            AttemptingBeeUnique = ""
            LocalCompletionStatus = fnssNotRerun
            GoTo FinishedOK
        End If
	If ScanRecoverState = "PagesExist" Then
            ErrorMessage = GetSubstitutedMessage(IDS_BMSG_BATCH_WITH_PAGES, IncomingRepObject.name)
            Junk = MsgBox(ErrorMessage, vbYesNo + vbQuestion + vbSystemModal, CID_TITLE_BAR)
            If Junk = vbYes Then 
			    ClearBatch IncomingRepObject
				ScanRecoverState = "None"
			Else
				ScanRecoverState = "KeepPages"
			End If
        End If
        PrintDebug("Operator wants rerun.")
        StartingBeeUnique = "_Begin"

    End If

    If StartingBeeUnique = "_Begin" Then
        GoTo Do_BeginWorker
    End If
    If StartingBeeUnique = "_End" Then
        GoTo Do_EndWorker
    End If
    If StartingBeeUnique = "Commit" Then
        GoTo DoCommitWorker
    End If
    If StartingBeeUnique = "Assembly" Then
        GoTo DoAssemblyWorker
    End If
    If StartingBeeUnique = "Image_Verify" Then
        GoTo DoImage_VerifyWorker
    End If
    If StartingBeeUnique = "Event_Activator" Then
        GoTo DoEvent_ActivatorWorker
    End If
    If StartingBeeUnique = "Scan_P0" Then
        GoTo DoScan_P0Worker
    End If
    If StartingBeeUnique = "Index" Then
        GoTo DoIndexWorker
    End If

    ErrorMessage = GetSubstitutedMessage(IDS_BMSG_INVALID_START, IncomingRepObject.name)
    GoTo DisplayErrorMessage

    
    '=============================================================
    ' Routing Concept implementation starts here!
    '-------------------------------------------------------------
    ' The sequence is as follows:
    '   BeeScan_P0
    '   BeeEvent_Activator
    '   BeeImage_Verify
    '   BeeAssembly
    '   BeeIndex
    '   BeeCommit
    '
    ' If you would prefer that on one line:
    '   (begin)->BeeScan_P0->BeeEvent_Activator->BeeImage_Verify->BeeAssembly->BeeIndex->BeeCommit
    '=============================================================

Do_BeginWorker:
    '----------------------------------------------
    ' This is the beginning of the routing CapturePath.
    '----------------------------------------------

    DebugIndent = 0
    PrintDebug("Arrived at '_Begin' phase.")
    DebugIndent = DebugIndent + 1
    AttemptingBeeUnique = "_Begin"
    If AgentRequestBee("_Begin") = fnspAbort Then
        GoTo CapturePathAborted
    End If

    LastCompleteBeeUnique = AttemptingBeeUnique


DoScan_P0Worker:
    '----------------------------------------------
    ' The Panagon Capture Scan phase.
    '----------------------------------------------
    ' Granularity: Doc
    ' Outputs:     one
    '----------------------------------------------

    DebugIndent = 0
    PrintDebug("Arrived at 'Scan_P0' phase.")
    DebugIndent = DebugIndent + 1
    PrintDebug(WorkArrayStatus())
    AttemptingBeeUnique = "Scan_P0"
    If AgentRequestBee("Scan_P0") = fnspAbort Then
        GoTo CapturePathAborted
    End If

    If BeeScan_P0 Is Nothing Then
        GoTo WorkerBeeUnavailable
    End If

    ' Tell the Worker Bee to actually do the work.
    PrintDebug("Calling 'BeeScan_P0.Execute()'.")
    ErrorMessage = "The Capture Path caught an exception while calling the Execute() method of 'Scan_P0'. It looks like 'Panagon Capture Scan' is having problems."
    'OutAlternativeCode = BeeScan_P0.Execute(WorkArray)
    OutAlternativeCode = CPExecute(BeeScan_P0, ScanRecoverState, IncomingRepObject)
    ErrorMessage = "Error trap after returning from 'BeeScan_P0.Execute()'."
    PrintDebug("'BeeScan_P0.Execute()' returned '" & OutAlternativeCode & "'.")
    If OutAlternativeCode = -1 Then
        GoTo WorkerBeeRejectedIt
    End If
    If OutAlternativeCode = 0 Then
        PrintDebug("'BeeScan_P0.Execute()' reported error.")
        ErrorMessage = "'BeeScan_P0.Execute()' reported error."
        GoTo Do_ErrorWorker
    End If
    PrintDebug("'BeeScan_P0.Execute()' reported success.")
    PrintDebug(WorkArrayStatus())
    LastCompleteBeeUnique = AttemptingBeeUnique

    ' Only output possibility is to fall through to next Bee.

DoEvent_ActivatorWorker:
    '----------------------------------------------
    ' The Panagon Capture Event Activator phase.
    '----------------------------------------------
    ' Granularity: Doc
    ' Outputs:     one
    '----------------------------------------------

    DebugIndent = 0
    PrintDebug("Arrived at 'Event_Activator' phase.")
    DebugIndent = DebugIndent + 1
    PrintDebug(WorkArrayStatus())
    AttemptingBeeUnique = "Event_Activator"
    If AgentRequestBee("Event_Activator") = fnspAbort Then
        GoTo CapturePathAborted
    End If

    If BeeEvent_Activator Is Nothing Then
        GoTo WorkerBeeUnavailable
    End If

    ' Tell the Worker Bee to actually do the work.
    PrintDebug("Calling 'BeeEvent_Activator.Execute()'.")
    ErrorMessage = "The Capture Path caught an exception while calling the Execute() method of 'Event_Activator'. It looks like 'Panagon Capture Event Activator' is having problems."
    'OutAlternativeCode = BeeEvent_Activator.Execute(WorkArray)
    OutAlternativeCode = CPExecute(BeeEvent_Activator, ScanRecoverState, IncomingRepObject)
    ErrorMessage = "Error trap after returning from 'BeeEvent_Activator.Execute()'."
    PrintDebug("'BeeEvent_Activator.Execute()' returned '" & OutAlternativeCode & "'.")
    If OutAlternativeCode = -1 Then
        GoTo WorkerBeeRejectedIt
    End If
    If OutAlternativeCode = 0 Then
        PrintDebug("'BeeEvent_Activator.Execute()' reported error.")
        ErrorMessage = "'BeeEvent_Activator.Execute()' reported error."
        GoTo Do_ErrorWorker
    End If
    PrintDebug("'BeeEvent_Activator.Execute()' reported success.")
    PrintDebug(WorkArrayStatus())
    LastCompleteBeeUnique = AttemptingBeeUnique

    ' Only output possibility is to fall through to next Bee.

DoImage_VerifyWorker:
    '----------------------------------------------
    ' The Panagon Capture Image Verify phase.
    '----------------------------------------------
    ' Granularity: Doc
    ' Outputs:
    '   Output code of '1' indicates 'OK' condition.
    '   Output code of '2' indicates 'Reject' condition.
    '----------------------------------------------

    DebugIndent = 0
    PrintDebug("Arrived at 'Image_Verify' phase.")
    DebugIndent = DebugIndent + 1
    PrintDebug(WorkArrayStatus())
    AttemptingBeeUnique = "Image_Verify"
    If AgentRequestBee("Image_Verify") = fnspAbort Then
        GoTo CapturePathAborted
    End If

    If BeeImage_Verify Is Nothing Then
        GoTo WorkerBeeUnavailable
    End If

    ' Tell the Worker Bee to actually do the work.
    PrintDebug("Calling 'BeeImage_Verify.Execute()'.")
    ErrorMessage = "The Capture Path caught an exception while calling the Execute() method of 'Image_Verify'. It looks like 'Panagon Capture Image Verify' is having problems."
    'OutAlternativeCode = BeeImage_Verify.Execute(WorkArray)
    OutAlternativeCode = CPExecute(BeeImage_Verify, ScanRecoverState, IncomingRepObject)
    ErrorMessage = "Error trap after returning from 'BeeImage_Verify.Execute()'."
    PrintDebug("'BeeImage_Verify.Execute()' returned '" & OutAlternativeCode & "'.")
    If OutAlternativeCode = -1 Then
        GoTo WorkerBeeRejectedIt
    End If
    If OutAlternativeCode = 0 Then
        PrintDebug("'BeeImage_Verify.Execute()' reported error.")
        ErrorMessage = "'BeeImage_Verify.Execute()' reported error."
        GoTo Do_ErrorWorker
    End If
    PrintDebug("'BeeImage_Verify.Execute()' reported success.")
    PrintDebug(WorkArrayStatus())
    LastCompleteBeeUnique = AttemptingBeeUnique

    ' Figure out where the normal output will send us next.
    If OutAlternativeCode = 2 Then
        GoTo DoAssemblyWorker
    End If
    ' Only other possibility is to fall through to next Bee.
    ' If you get here, then OutAlternativeCode should be "1".
    If OutAlternativeCode <> 1 Then
        ErrorMessage = "'Image_Verify.Execute()' took unexpected output alternative."
        GoTo Do_ErrorWorker
    End If
    ' Fall through to next Bee.

DoAssemblyWorker:
    '----------------------------------------------
    ' The Panagon Capture Assembly phase.
    '----------------------------------------------
    ' Granularity: Doc
    ' Outputs:
    '   Output code of '1' indicates 'OK' condition.
    '   Output code of '2' indicates 'Empty' condition.
    '----------------------------------------------

    DebugIndent = 0
    PrintDebug("Arrived at 'Assembly' phase.")
    DebugIndent = DebugIndent + 1
    PrintDebug(WorkArrayStatus())
    AttemptingBeeUnique = "Assembly"
    If AgentRequestBee("Assembly") = fnspAbort Then
        GoTo CapturePathAborted
    End If

    If BeeAssembly Is Nothing Then
        GoTo WorkerBeeUnavailable
    End If

    ' Tell the Worker Bee to actually do the work.
    PrintDebug("Calling 'BeeAssembly.Execute()'.")
    ErrorMessage = "The Capture Path caught an exception while calling the Execute() method of 'Assembly'. It looks like 'Panagon Capture Assembly' is having problems."
    'OutAlternativeCode = BeeAssembly.Execute(WorkArray)
    OutAlternativeCode = CPExecute(BeeAssembly, ScanRecoverState, IncomingRepObject)
    ErrorMessage = "Error trap after returning from 'BeeAssembly.Execute()'."
    PrintDebug("'BeeAssembly.Execute()' returned '" & OutAlternativeCode & "'.")
    If OutAlternativeCode = -1 Then
        GoTo WorkerBeeRejectedIt
    End If
    If OutAlternativeCode = 0 Then
        PrintDebug("'BeeAssembly.Execute()' reported error.")
        ErrorMessage = "'BeeAssembly.Execute()' reported error."
        GoTo Do_ErrorWorker
    End If
    PrintDebug("'BeeAssembly.Execute()' reported success.")
    PrintDebug(WorkArrayStatus())
    LastCompleteBeeUnique = AttemptingBeeUnique

    ' Figure out where the normal output will send us next.
    If OutAlternativeCode = 2 Then
        GoTo DoScan_P0Worker
    End If
    ' Only other possibility is to fall through to next Bee.
    ' If you get here, then OutAlternativeCode should be "1".
    If OutAlternativeCode <> 1 Then
        ErrorMessage = "'Assembly.Execute()' took unexpected output alternative."
        GoTo Do_ErrorWorker
    End If
    ' Fall through to next Bee.

DoIndexWorker:
    '----------------------------------------------
    ' The Panagon Capture Index Control phase.
    '----------------------------------------------
    ' Granularity: Doc
    ' Outputs:     one
    '----------------------------------------------

    DebugIndent = 0
    PrintDebug("Arrived at 'Index' phase.")
    DebugIndent = DebugIndent + 1
    PrintDebug(WorkArrayStatus())
    AttemptingBeeUnique = "Index"
    If AgentRequestBee("Index") = fnspAbort Then
        GoTo CapturePathAborted
    End If

    If BeeIndex Is Nothing Then
        GoTo WorkerBeeUnavailable
    End If

    ' Tell the Worker Bee to actually do the work.
    PrintDebug("Calling 'BeeIndex.Execute()'.")
    ErrorMessage = "The Capture Path caught an exception while calling the Execute() method of 'Index'. It looks like 'Panagon Capture Index Control' is having problems."
    'OutAlternativeCode = BeeIndex.Execute(WorkArray)
    OutAlternativeCode = CPExecute(BeeIndex, ScanRecoverState, IncomingRepObject)
    ErrorMessage = "Error trap after returning from 'BeeIndex.Execute()'."
    PrintDebug("'BeeIndex.Execute()' returned '" & OutAlternativeCode & "'.")
    If OutAlternativeCode = -1 Then
        GoTo WorkerBeeRejectedIt
    End If
    If OutAlternativeCode = 0 Then
        PrintDebug("'BeeIndex.Execute()' reported error.")
        ErrorMessage = "'BeeIndex.Execute()' reported error."
        GoTo Do_ErrorWorker
    End If
    PrintDebug("'BeeIndex.Execute()' reported success.")
    PrintDebug(WorkArrayStatus())
    LastCompleteBeeUnique = AttemptingBeeUnique

    ' Only output possibility is to fall through to next Bee.

DoCommitWorker:
    '----------------------------------------------
    ' The Panagon Capture Commit phase.
    '----------------------------------------------
    ' Granularity: Doc
    ' Outputs:     one
    '----------------------------------------------

    DebugIndent = 0
    PrintDebug("Arrived at 'Commit' phase.")
    DebugIndent = DebugIndent + 1
    PrintDebug(WorkArrayStatus())
    AttemptingBeeUnique = "Commit"
    If AgentRequestBee("Commit") = fnspAbort Then
        GoTo CapturePathAborted
    End If

    If BeeCommit Is Nothing Then
        GoTo WorkerBeeUnavailable
    End If

    ' Tell the Worker Bee to actually do the work.
    PrintDebug("Calling 'BeeCommit.Execute()'.")
    ErrorMessage = "The Capture Path caught an exception while calling the Execute() method of 'Commit'. It looks like 'Panagon Capture Commit' is having problems."
    'OutAlternativeCode = BeeCommit.Execute(WorkArray)
    OutAlternativeCode = CPExecute(BeeCommit, ScanRecoverState, IncomingRepObject)
    ErrorMessage = "Error trap after returning from 'BeeCommit.Execute()'."
    PrintDebug("'BeeCommit.Execute()' returned '" & OutAlternativeCode & "'.")
    If OutAlternativeCode = -1 Then
        GoTo WorkerBeeRejectedIt
    End If
    If OutAlternativeCode = 0 Then
        PrintDebug("'BeeCommit.Execute()' reported error.")
        ErrorMessage = "'BeeCommit.Execute()' reported error."
        GoTo Do_ErrorWorker
    End If
    PrintDebug("'BeeCommit.Execute()' reported success.")
    PrintDebug(WorkArrayStatus())
    LastCompleteBeeUnique = AttemptingBeeUnique

    ' Only output possibility is to fall through to next Bee.


    '----------------------------------------------
    ' Straight-line Routing Concept implementation ends here!
    '----------------------------------------------

    '----------------------------------------------
    ' The End phase.
    '----------------------------------------------
    ' Granularity: Doc
    ' Outputs:     one
    '----------------------------------------------
    '
    ' CapturePaths come here when their execution is complete.
    '
    ' When a CapturePath ends by coming here, it receives a distinctly different
    ' status than a CapturePath that goes to the "Error" node.
    '----------------------------------------------

Do_EndWorker:
    DebugIndent = 0
    PrintDebug("Arrived at '_End' phase.")
    DebugIndent = DebugIndent + 1
    If AgentRequestBee("_End") = fnspAbort Then
        GoTo CapturePathAborted
    End If

    LastCompleteBeeUnique = "_End"

    ' Mark next phase as "stopped after successful completion".
    AttemptingBeeUnique = CID_NAME_DONE_FLAG

    ' Set status for the router.
    LocalCompletionStatus = fnssCompletedOk

    ' Everything worked OK.
    GoTo FinishedOK


    '=============================================================
    ' Non-linear Routing Concept implementation starts here!
    '=============================================================



    '----------------------------------------------
    ' The Error phase.
    '----------------------------------------------
    ' Granularity: Doc
    ' Outputs:     one
    '----------------------------------------------
    ' Errors fall into two classes:  recoverable and nonrecoverable.
    '
    ' It is assumed that each Worker Bee takes care of his own recoverable
    ' errors.  Thus the Scan Worker Bee takes care of reporting and clearing
    ' paper jams.  After recovering, execution continues, so that the VBA
    ' CapturePath doesn't even know about it and we don't get here.
    '
    ' All of the nonrecoverable errors get funneled here.  This code assumes
    ' that the Bee has already put up a suitable error message.  It might be
    ' nice to someday move all of that error reporting code into the CapturePath
    ' and just print the error information here.  But some Bees might want
    ' to stop the CapturePath without a real error, so any future code that pops
    ' dialogs from here would have to check for a SUCCESS and skip the
    ' dialog.
    '
    ' When a CapturePath ends by coming here, it receives a distinctly different
    ' status than a CapturePath that goes to the normal "End".
    '----------------------------------------------

Do_ErrorWorker:
    DebugIndent = 0
    PrintDebug("Arrived at '_Error' phase.")
    DebugIndent = DebugIndent + 1
    If AgentRequestBee("_Error") = fnspAbort Then
        GoTo CapturePathAborted
    End If

    LastCompleteBeeUnique = "_Error"

    ' Leave "AttemptingBeeUnique" so user can resume later.

    ' Set status for the router.
    LocalCompletionStatus = fnssStoppedError

    ' The error has already been reported.
    GoTo FinishedError


    '----------------------------------------------
    ' Non-linear Routing Concept implementation ends here!
    '----------------------------------------------


    '=============================================================
    ' Recovery and termination mechanisms.
    '=============================================================


    '----------------------------------------------
    ' The following code takes care of a Batch Flow CapturePath that requires
    ' a Worker Bee that does not exist on the current machine.  This
    ' facilitates running _part_ of a Batch Flow CapturePath on one machine
    ' and the rest elsewhere.
    '
    ' Before jumping here, AttemptingBeeUnique will have been set to reflect
    ' the Bee that is not installed.
    '----------------------------------------------

WorkerBeeUnavailable:
    DebugIndent = 0
    PrintDebug("Worker Bee '" & AttemptingBeeUnique & "' is not available on this machine.")
	
	ErrorMessage = GetSubstitutedMessage(IDS_BMSG_UNAVAILABLE_BEE, AttemptingBeeUnique)
    Junk = MsgBox(ErrorMessage, vbOKOnly + vbSystemModal, CID_TITLE_BAR)
   
    ' Leave "AttemptingBeeUnique" so user can resume later.

    ' Set status for the router.
    LocalCompletionStatus = fnssSuspendedMissing

    ' This is not an error.
    GoTo FinishedOK


    '----------------------------------------------
    ' The following code takes care of a situation where a Worker Bee refuses to
    ' process or complete processing on a Batch.  This could be used as a filtering
    ' mechanism or part of a "complete it later" option.
    '
    ' Before jumping here, AttemptingBeeUnique will have been set to reflect
    ' the Bee that is to be (eventually) retried.
    '----------------------------------------------

WorkerBeeRejectedIt:
    DebugIndent = 0
    PrintDebug("Worker Bee '" & AttemptingBeeUnique & "' has rejected this Batch.")

    ' Leave "AttemptingBeeUnique" so user can resume later.

    ' Set status for the router.
    LocalCompletionStatus = fnssSuspendedBeeReject

    ' This is not an error.
    GoTo FinishedOK


    '----------------------------------------------
    ' The following code takes care of a Batch Flow CapturePath that hits a Force
    ' Machine Transition node.  This
    ' facilitates running _part_ of a Batch Flow CapturePath on one machine
    ' and the rest elsewhere.
    '
    ' Before jumping here, AttemptingBeeUnique will have been set to reflect
    ' the Bee that is not installed.
    '----------------------------------------------

DidTransition:
    DebugIndent = 0
    PrintDebug("Worker Bee '" & LastCompleteBeeUnique & "' forces machine transition to resume at '" & AttemptingBeeUnique & "'.")

    ' "AttemptingBeeUnique" will be used in cleanup for the the new position.

    ' Set status for the router.
    LocalCompletionStatus = fnssSuspendedTransit

    ' This is not an error.
    GoTo FinishedOK


    '----------------------------------------------
    ' The following code services the error trap.
    '
    ' This catches errors "thrown" by C++ components as well as
    ' certain VB runtime errors.  VB's Error Object should give us
    ' details.
    '----------------------------------------------

ErrorTrap:
    ' Turn off error trap so we don't loop in the event of a subsequent error.
    On Error Resume Next

    DebugIndent = 0
    ErrorMessage = ErrorMessage & "  " & Err.Description
    PrintDebug("Error Trap!")
    PrintDebug("Error:  '" & ErrorMessage & "'")
    Junk = MsgBox(ErrorMessage, vbOKOnly + vbCritical + vbSystemModal, CID_TITLE_BAR)

    ' Leave "AttemptingBeeUnique" so user can resume later.

    ' Set status for the router.
    LocalCompletionStatus = fnssStoppedTrap

    GoTo FinishedError


    '----------------------------------------------
    ' The following code takes care of CapturePaths that have been aborted by the user.
    '----------------------------------------------

CapturePathAborted:

    DebugIndent = 0
    PrintDebug("CapturePath execution aborted!")

    ' Leave "AttemptingBeeUnique", even though there is no way to resume later.

    ' Set status for the router.
    LocalCompletionStatus = fnssStoppedAbort

    GoTo FinishedError


    '----------------------------------------------
    ' The following code takes care of defective Worker Bees.
    '----------------------------------------------

WorkerBeeDefective:             ' Bee failed interface sanity test.

WorkerBeeTookBadOutput:         ' Bee said "N" output alternatives but took something different.

    DebugIndent = 0
    PrintDebug("Worker Bee is defective!")
    PrintDebug("Error:  '" & ErrorMessage & "'")
    Junk = MsgBox(ErrorMessage, vbOKOnly + vbCritical + vbSystemModal, CID_TITLE_BAR)

    ' Leave "AttemptingBeeUnique" so user can resume later.

    ' Set status for the router.
    LocalCompletionStatus = fnssStoppedBadBee

    GoTo FinishedError


    '----------------------------------------------
    ' The following code takes care of errors detected in the CapturePath, with
    ' descriptive text placed in ErrorMessage .
    '----------------------------------------------

DisplayErrorMessage:

    DebugIndent = 0
    PrintDebug("CapturePath detected an error!")
    PrintDebug("Error:  '" & ErrorMessage & "'")
    Junk = MsgBox(ErrorMessage, vbOKOnly + vbCritical + vbSystemModal, CID_TITLE_BAR)

    GoTo FinishedError


    '=============================================================
    ' Cleanup starts here!
    '=============================================================

    While False
FinishedOK:
        WorkedOK = True
    Wend

    While False
FinishedError:
        WorkedOK = False
        ' We assume that any necessary error messages have been displayed.
    Wend

    ' For one reason or another, we have finished any work that we might do.
    ' Everybody gets funneled through here.
            
    ' Turn off error trap so we don't loop in the event of a subsequent error.
    On Error Resume Next
    DebugIndent = 0
  
    ' If we made any progress at all, let it be known.
    PrintDebug("Work is complete, for one reason or another.")
    DebugIndent = DebugIndent + 1
    PrintDebug(WorkArrayStatus())
    PrintDebug("Last complete phase is '" & LastCompleteBeeUnique & "'.")
    PrintDebug("Next phase is '" & AttemptingBeeUnique & "'.")
    DebugIndent = DebugIndent - 1

    ' Mark the work with completion status.
    '   LocalCompletionStatus    NextPhase          StartBee
    '   ---------------------    -----------------  --------------
    '   fnssCompletedOk          set by last Bee    CID_NAME_DONE_FLAG
    '   fnssStoppedError         Bee that failed    Bee that failed
    '   fnssSuspendedMissing     Bee that failed    Bee that failed
    '   fnssSuspendedBeeReject   Bee that failed    Bee that failed
    '   fnssSuspendedTransit     Bee that failed    Bee that failed
    '   fnssStoppedTrap          Bee that failed    CID_NAME_ERROR_FLAG
    '   fnssStoppedBadBee        Bee that failed    CID_NAME_ERROR_FLAG
    '   fnssStoppedAbort         no change          CID_NAME_ABORT_FLAG
    '   fnssNotRerun             no change          no change
    PrintDebug("Marking completion status...")
    DebugIndent = DebugIndent + 1
    If LocalCompletionStatus = fnssCompletedOk Then
        PushWorkArray -1, CID_NAME_DONE_FLAG, False
        PushBatch IncomingRepObject, -1, CID_NAME_DONE_FLAG, False
    End If
    If LocalCompletionStatus = fnssStoppedError Or LocalCompletionStatus = fnssSuspendedMissing Or LocalCompletionStatus = fnssSuspendedBeeReject Or LocalCompletionStatus = fnssSuspendedTransit Then
        PushWorkArray MappedPhase(AttemptingBeeUnique), AttemptingBeeUnique, False
        PushBatch IncomingRepObject, MappedPhase(AttemptingBeeUnique), AttemptingBeeUnique, False
    End If
    If LocalCompletionStatus = fnssStoppedTrap Or LocalCompletionStatus = fnssStoppedBadBee Then
        PushWorkArray MappedPhase(AttemptingBeeUnique), CID_NAME_ERROR_FLAG, False
        PushBatch IncomingRepObject, MappedPhase(AttemptingBeeUnique), CID_NAME_ERROR_FLAG, False
    End If
    If LocalCompletionStatus = fnssStoppedAbort Then
        PushWorkArray -1, CID_NAME_ABORT_FLAG, False
        PushBatch IncomingRepObject, -1, CID_NAME_ABORT_FLAG, False
    End If
    If LocalCompletionStatus = fnssNotRerun Then
        PushBatch IncomingRepObject, -1, "", False
    End If
    PrintDebug("Done marking completion status.")
    DebugIndent = DebugIndent - 1
    
    If WorkedOK Then
        ' Everything's fine.
        ' Let the Bees know that the CapturePath is over.
        ' Note that we use SetExecutionMode(fnemAbort) because even though there is no error, we
        ' demand that execution cease - we will not be going back in.
        PrintDebug("Telling all Bees that the CapturePath is over...")
        DebugIndent = DebugIndent + 1
        If BeeCommit Is Nothing Then
            PrintDebug("Unavailable 'BeeCommit'.")
        Else
            PrintDebug("Calling 'BeeCommit.SetExecutionMode(fnemAbort)'.")
            BeeCommit.SetExecutionMode(fnemAbort)
        End If
        If BeeAssembly Is Nothing Then
            PrintDebug("Unavailable 'BeeAssembly'.")
        Else
            PrintDebug("Calling 'BeeAssembly.SetExecutionMode(fnemAbort)'.")
            BeeAssembly.SetExecutionMode(fnemAbort)
        End If
        If BeeImage_Verify Is Nothing Then
            PrintDebug("Unavailable 'BeeImage_Verify'.")
        Else
            PrintDebug("Calling 'BeeImage_Verify.SetExecutionMode(fnemAbort)'.")
            BeeImage_Verify.SetExecutionMode(fnemAbort)
        End If
        If BeeEvent_Activator Is Nothing Then
            PrintDebug("Unavailable 'BeeEvent_Activator'.")
        Else
            PrintDebug("Calling 'BeeEvent_Activator.SetExecutionMode(fnemAbort)'.")
            BeeEvent_Activator.SetExecutionMode(fnemAbort)
        End If
        If BeeScan_P0 Is Nothing Then
            PrintDebug("Unavailable 'BeeScan_P0'.")
        Else
            PrintDebug("Calling 'BeeScan_P0.SetExecutionMode(fnemAbort)'.")
            BeeScan_P0.SetExecutionMode(fnemAbort)
        End If
        If BeeIndex Is Nothing Then
            PrintDebug("Unavailable 'BeeIndex'.")
        Else
            PrintDebug("Calling 'BeeIndex.SetExecutionMode(fnemAbort)'.")
            BeeIndex.SetExecutionMode(fnemAbort)
        End If
        PrintDebug("Done telling all Bees that the CapturePath is over.")
        DebugIndent = DebugIndent - 1
    Else
        ' Something bad happened.
        ' Let the Worker Bees know so that they can clean up.
        PrintDebug("Telling all Bees that there was an error...")
        DebugIndent = DebugIndent + 1
        If BeeCommit Is Nothing Then
            PrintDebug("Unavailable 'BeeCommit'.")
        Else
            PrintDebug("Calling 'BeeCommit.SetExecutionMode(fnemAbort)'.")
            BeeCommit.SetExecutionMode(fnemAbort)
        End If
        If BeeAssembly Is Nothing Then
            PrintDebug("Unavailable 'BeeAssembly'.")
        Else
            PrintDebug("Calling 'BeeAssembly.SetExecutionMode(fnemAbort)'.")
            BeeAssembly.SetExecutionMode(fnemAbort)
        End If
        If BeeImage_Verify Is Nothing Then
            PrintDebug("Unavailable 'BeeImage_Verify'.")
        Else
            PrintDebug("Calling 'BeeImage_Verify.SetExecutionMode(fnemAbort)'.")
            BeeImage_Verify.SetExecutionMode(fnemAbort)
        End If
        If BeeEvent_Activator Is Nothing Then
            PrintDebug("Unavailable 'BeeEvent_Activator'.")
        Else
            PrintDebug("Calling 'BeeEvent_Activator.SetExecutionMode(fnemAbort)'.")
            BeeEvent_Activator.SetExecutionMode(fnemAbort)
        End If
        If BeeScan_P0 Is Nothing Then
            PrintDebug("Unavailable 'BeeScan_P0'.")
        Else
            PrintDebug("Calling 'BeeScan_P0.SetExecutionMode(fnemAbort)'.")
            BeeScan_P0.SetExecutionMode(fnemAbort)
        End If
        If BeeIndex Is Nothing Then
            PrintDebug("Unavailable 'BeeIndex'.")
        Else
            PrintDebug("Calling 'BeeIndex.SetExecutionMode(fnemAbort)'.")
            BeeIndex.SetExecutionMode(fnemAbort)
        End If
        PrintDebug("Done telling all Bees that there was an error.")
        DebugIndent = DebugIndent - 1
    End If
    
    ' Get rid of the Worker Bees.
    ' This will release the Worker Bee references that we borrowed from the Agent.
    PrintDebug("Releasing CapturePath's copy of Bee dispatch pointers.")
    Set BeeCommit = Nothing
    Set BeeAssembly = Nothing
    Set BeeImage_Verify = Nothing
    Set BeeEvent_Activator = Nothing
    Set BeeScan_P0 = Nothing
    Set BeeIndex = Nothing
    
    ' Get rid of assorted junk.
    Set Probe = Nothing

    ' Release everything in the work array.
    PrintDebug("Releasing WorkArray's " & (UBound(WorkArray, 1) - LBound(WorkArray, 1) +1 ) & " element(s).")
    DebugIndent = DebugIndent + 1
    For iIndex = LBound(WorkArray, 1) To UBound(WorkArray, 1)
        Set WorkArray(iIndex) = Nothing
    Next iIndex
    Call MakeNilWorkArray
    PrintDebug(WorkArrayStatus())
    DebugIndent = DebugIndent - 1

    ' Release the document hierarchy.
    Set RepRoot = Nothing
    Set IncomingRepObject = Nothing

    ' Tell the Routing Agent our disposition.
    AgentCompletionStatus = LocalCompletionStatus
    
    ' All done.
    PrintDebug("CapturePath exiting.")
End Sub ' Main()


Sub PrintDebug(InfoLine As String)
    AgentDebugString(Space(DebugIndent * 2) & InfoLine)
End Sub


' This function gets error message text and substitutes in one string.
Function GetSubstitutedMessage(lMessageId as Long, Filling As String) As String
	Dim BaseMessage As String
	Dim InsertionPoint As Integer

	' Get the base string from the resource table of the Routing Agent.
	BaseMessage = AgentGetMessage(lMessageId)

	GetSubstitutedMessage = ReplaceSubstring(BaseMessage, "%s", Filling)
End Function ' GetSubstitutedMessage()


' Perform a substring substitution.
Function ReplaceSubstring(OriginalString As String, ToFind As String, ToInsert As String) As String
    Dim FoundLoc As Integer
    
    FoundLoc = InStr(OriginalString, ToFind)
    If FoundLoc > 0 Then
        ReplaceSubstring = Left(OriginalString, FoundLoc - 1) & ToInsert & Mid(OriginalString, FoundLoc + Len(ToFind))
    Else
        ReplaceSubstring = OriginalString
    End If
End Function ' ReplaceSubstring()


' This function sets the WorkArray to the NIL state.
Sub MakeNilWorkArray()
    ReDim WorkArray(1)
    Set WorkArray(1) = Nothing
End Sub ' MakeNilWorkArray()


' Returns a boolean indicating whether or not the WorkArray is NIL.
Function WorkArrayIsNil() As Boolean
    On Error GoTo ErrorTrap

    If ( UBound(WorkArray, 1) - LBound(WorkArray, 1) +1 ) = 1 And WorkArray(1) Is Nothing Then
        WorkArrayIsNil = True
    Else
        WorkArrayIsNil = False
    End If

    While False
ErrorTrap:
        PrintDebug("WorkArray is corrupt!")
    Wend
End Function ' WorkArrayIsNil()


' Used to fill a string with the status of the WorkArray.
Function WorkArrayStatus() As String
    On Error GoTo ErrorTrap

    If WorkArrayIsNil() Then
        WorkArrayStatus = "WorkArray is NIL."
    Else
        WorkArrayStatus = "WorkArray contains " & ( UBound(WorkArray, 1) - LBound(WorkArray, 1) +1 ) & " element(s)."
    End If

    While False
ErrorTrap:
        WorkArrayStatus = "WorkArray is corrupt!"
    Wend
End Function ' WorkArrayStatus()


' Used to map from the unique Bee name to a BES phase number.
Function MappedPhase(BeeUniqueName As String) As Integer
    MappedPhase = 0
    If BeeUniqueName = "_Begin" Then MappedPhase = 0
    If BeeUniqueName = "_Error" Then MappedPhase = -1
    If BeeUniqueName = "_End" Then MappedPhase = -1
    If BeeUniqueName = "Commit" Then MappedPhase = 8
    If BeeUniqueName = "Assembly" Then MappedPhase = 4
    If BeeUniqueName = "Image_Verify" Then MappedPhase = 2
    If BeeUniqueName = "Event_Activator" Then MappedPhase = 20
    If BeeUniqueName = "Scan_P0" Then MappedPhase = 1
    If BeeUniqueName = "Index" Then MappedPhase = 5
End Function ' MappedPhase()


' This function takes everything in the WorkArray and marks it to go on to the
' specified phase.
'   NextPhaseNumber - Phase number to assign. -1 means don't touch.
'   NextBeeUniqueName - StartBee name to assign. "" means don't touch.
'   EmptyBatch - If TRUE, we do a Clear() on each element of the Work Array. This code assumes that
'      this is _not_ the seed Batch, which is already locked by the Routing Agent.
Sub PushWorkArray(NextPhaseNumber As Integer, NextBeeUniqueName As String, EmptyBatch As Boolean)
    Dim WorkItemV As Variant
    Dim WorkItem As Object
    Dim WorkedOK As Boolean
    Dim iIndex As Integer

    If WorkArrayIsNil() Then
        PrintDebug("Pushing a NIL WorkArray.")
    Else
        PrintDebug("Pushing WorkArray...")
        DebugIndent = DebugIndent + 1
        PrintDebug(WorkArrayStatus())

        ' Set each bit of work to go on to the next phase.
        For Each WorkItemV In WorkArray
            Set WorkItem = WorkItemV
            PushBatch WorkItem, NextPhaseNumber, NextBeeUniqueName, EmptyBatch
            Set WorkItem = Nothing
        Next WorkItemV

        ' Release the reference on each object.
        For iIndex = LBound(WorkArray, 1) To UBound(WorkArray, 1)
            Set WorkArray(iIndex) = Nothing
        Next iIndex
        Call MakeNilWorkArray

        PrintDebug(WorkArrayStatus())
        DebugIndent = DebugIndent - 1
    End If
End Sub ' PushWorkArray()


' This function takes a Batch and marks it to go on to the specified phase.
'   NextPhaseNumber - Phase number to assign. -1 means don't touch.
'   NextBeeUniqueName - StartBee name to assign. "" means don't touch.
'   EmptyBatch - If TRUE, we do a Clear() on each element of the Work Array. This code assumes that
'      this is _not_ the seed Batch, which is already locked by the Routing Agent.
Sub PushBatch(BatchObject As Object, NextPhaseNumber As Integer, NextBeeUniqueName As String, EmptyBatch As Boolean)
    Dim WorkedOK As Boolean

    PrintDebug("Pushing Batch...")
    DebugIndent = DebugIndent + 1

    If BatchObject Is Nothing Then
        PrintDebug("Pushing a NULL batch.")
        GoTo ExitPoint
    End If

    ' If the Batch is either being committed or has already been committed, skip it.
    If BatchObject.Status = fnrsBusy Or BatchObject.Status = fnrsDetached Then
        PrintDebug("Batch was committed.")
        GoTo ExitPoint
    End If

    If EmptyBatch Then
        PrintDebug("Clearing '" & BatchObject.Name & "'.")
        ' 980115 - Gary says Batch must not be locked to clear it. This will not work on seed batch.
        BatchObject.Clear
    End If
    BatchObject.Lock
    PrintDebug("Pushing '" & BatchObject.Name & "' to '" & NextBeeUniqueName & "' PhaseNumber " & NextPhaseNumber & ".")
    If NextPhaseNumber <> -1 Then
        BatchObject.NextPhase = NextPhaseNumber
    End If
    If NextBeeUniqueName <> "" Then
        WorkedOK = SetAttribute(BatchObject, CID_GROUP_CONTROL, CID_NAME_STARTING_PHASE, NextBeeUniqueName)
    End If
    BatchObject.Save
    BatchObject.Unlock

ExitPoint:
    DebugIndent = DebugIndent - 1
End Sub ' PushBatch()


' This function takes a Batch and clears it.
'
' If this is the original Batch that is attached to this CapturePath, it has already been
' locked once by the Routing Agent. We must temporarily undo that lock in order for the
' clear to work.
' 980115 - Gary says Batch must NOT be locked to clear it.
Sub ClearBatch(BatchObject As Object)
    BatchObject.Unlock
    BatchObject.Clear
    BatchObject.Lock
End Sub ' ClearBatch()


' This function encapsulates any grodyness associated with getting an Attribute.
' Parameters are all mandatory:
'   AttClass    Name of the category into which the Attribute fits, such as
'               "Scan".
'   AttName     Name of the particular Attribute of AttClass category to look
'               for, such as "Duplex".
' Return value:
'   A string containing the value.
'   An empty string means that the Attribute does not exist.
Function GetAttribute(Parent As Object, AttClass As Long, AttName As Long)
    Dim Attrib As Object
    
    PrintDebug("Reading attribute on '" & Parent.Name & "':  Group='" & AttClass & "', Name='" & AttName & "'")

    ' Initialization
    On Error GoTo Trapped
    
    Set Attrib = Parent.Attributes(AttClass, AttName)

    ' FOR COMPATIBILITY WITH pre-2.0 PANAMA, check for the text version of our attribute.
    ' REMOVE THIS for the 3.0 release.
    Dim CID_NAME_CONTROL As String
    Dim CID_NAME_START_BEE_UNIQUE As String
    CID_NAME_CONTROL = "Control"
    CID_NAME_START_BEE_UNIQUE = "StartingPhase"
    If Attrib Is Nothing Then
        Set Attrib = Parent.Attributes(CID_NAME_CONTROL, CID_NAME_START_BEE_UNIQUE)
    End If
	' END OF COMPATIBILITY

    If Attrib Is Nothing Then
        GetAttribute = ""    ' Return: failure.
        PrintDebug("...No such attribute.")
    Else
        GetAttribute = Attrib.Value
        PrintDebug("...Value='" & GetAttribute & "'")
    End If
      
    While False  
Trapped:
        PrintDebug("...Trapped getting attribute.")
        GetAttribute = ""    ' Return: failure.
    Wend

    ' Clean up.
    Set Attrib = Nothing
End Function ' GetAttribute()


' This function encapsulates any grodyness associated with Attribute setting.
' Parameters are all mandatory:
'   AttClass    Name of the category into which the Attribute fits, such as
'               "Scan".
'   AttName     Name of the particular Attribute of AttClass category to look
'               for, such as "Duplex".
'   AttValue    Value to write into the Attribute.
' Return value:
'   True        If everything worked OK.
'   False       If some error happened.
Function SetAttribute(Parent As Object, AttClass As Long, AttName As Long, AttValue As Variant) As Boolean
    Dim Attrib As Object
    Dim IsLocked As Boolean
    
    PrintDebug("Writing attribute on '" & Parent.Name & "':  Group='" & AttClass & "', Name='" & AttName & "', Value='" & AttValue & "'")

    ' Initialization
    IsLocked = False
    On Error GoTo Finished
    
    SetAttribute = True         ' Default: return success.

    IsLocked = Parent.Lock
    
    Set Attrib = Parent.Attributes(AttClass, AttName)
    If Attrib Is Nothing Then
	' Params are: Group, Name, Value.
	' Example:  batch.Attributes.Add("Stats","Creator","Gary")
        Set Attrib = Parent.Attributes.Add(AttClass, AttName, AttValue)
        PrintDebug("...Attribute added.")
    Else
        ' It exists already.
        Attrib.Value = AttValue
        PrintDebug("...Attribute rewritten.")
    End If
        
    Parent.Save

Finished:

    If Err.Number <> 0 Then
        SetAttribute = False    ' Return: failure.
    End If
    
    ' Clean up.
    Set Attrib = Nothing
    If IsLocked Then
        Parent.Unlock
        IsLocked = False
    End If
End Function ' SetAttribute()

Function CPExecute(CurrentWorkerBee As Object, ScanRecoverState As String, IncomingRepObject As Object) As Variant
    If CurrentWorkerBee.PhaseName = "Scan" Then
		CPExecute = ScanExecute(CurrentWorkerBee, ScanRecoverState, IncomingRepObject)
	Else
		If ScanRecoverState = "PagesExist" Then ScanRecoverState = ""
		CPExecute = CurrentWorkerBee.Execute(WorkArray)
	End If
End Function ' CPExecute()

Function ScanExecute(CurrentWorkerBee As Object, ScanRecoverState As String, IncomingRepObject As Object) As Variant
    Dim oRepObj As Object
    Dim j As Integer
	Dim ErrorMessage As String
	Dim Junk

    If ScanRecoverState = "PagesExist" Then
        PrintDebug("This Batch already has existing pages.")
        ErrorMessage = GetSubstitutedMessage(IDS_BMSG_BATCH_WITH_PAGES, IncomingRepObject.name)
        Junk = MsgBox(ErrorMessage, vbYesNo + vbQuestion + vbSystemModal, CID_TITLE_BAR)
        If Junk = vbYes Then
            ScanRecoverState = "None"
            ClearBatch IncomingRepObject
        Else
            ScanRecoverState = "KeepPages"
		End If
	End If

	Select Case ScanRecoverState
    Case "None"

    Case "KeepPages"  
		If Not IsScanner(CurrentWorkerBee) Then
			ScanRecoverState = "None"
		Else
			ScanRecoverState = "Recovered"
			ReDim WorkArray(IncomingRepObject.Contents.Count)
			j = 1
			For Each oRepObj In IncomingRepObject.Contents
				Set WorkArray(j) = oRepObj
				j = j + 1
			Next
		End If
		
	Case "Recovered"
		ReDim WorkArray(1)
        ScanRecoverState = "None"
        Set WorkArray(1) = IncomingRepObject
	
    Case Else

    End Select

	If ScanRecoverState = "Recovered" Then
	    ScanExecute = 1
	Else
        ScanExecute = CurrentWorkerBee.Execute(WorkArray)
	End If

End Function ' ScanExecute()

Function IsScanner(CurrentWorkerBee As Object) As Boolean
    On Error GoTo ErrorTrap
    Dim varDirectory As Variant
	IsScanner = True
    varDirectory = CurrentWorkerBee.Directory
	If Not IsEmpty(varDirectory) Then
		IsScanner = False
    End If
ErrorTrap:
    
End Function ' IsScanner()

