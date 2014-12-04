local ContinueText = GetLocalizedString("Tutorial_BtnContinue")

local TutorialManager = World:getTutorialManager()

function RunTutorial_Welcome()
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_Welcome_Step1"), ContinueText))
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_Welcome_Step2"), ContinueText))

	-- запускаем цепочки туториалов
	TutorialManager:addTutorialState("FirstCell")
	TutorialManager:addTutorialState("WaitForFirstInvestigator");
end

function RunTutorial_AfterCreatingFirstCell()
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_AfterCreatingFirstCell"), ContinueText))

	TutorialManager:addTutorialState("StartFirstTask");
	TutorialManager:removeTutorialState("FirstCell");
end

function RunTutorial_BeforeStartFirstTask()
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_BeforeStartFirstTask"), ContinueText))

	TutorialManager:addTutorialState("WaitingForStartFirstTask");
	TutorialManager:removeTutorialState("StartFirstTask");
end

function RunTutorial_StartingFirstTask()
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_StartingFirstTask"), ContinueText))

	TutorialManager:addTutorialState("WaitingForFinishFirstTask");
	TutorialManager:removeTutorialState("WaitingForStartFirstTask");
end

function RunTutorial_AfterFirstTaskFinished()
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_AfterFirstTaskFinished"), ContinueText))

	TutorialManager:addTutorialState("ReadyToFirstRealWork");
	TutorialManager:removeTutorialState("WaitingForFinishFirstTask");
end

function RunTutorial_StartingFirstRealWork()
	-- пока отключаем туториал про бустеры
	--TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_StartingFirstRealWork"), ContinueText))

	TutorialManager:addTutorialState("ReadyToFinishFirstRealWork");
	TutorialManager:removeTutorialState("ReadyToFirstRealWork");
end

function RunTutorial_AfterRealWorkDone()
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_AfterRealWorkDone"), ContinueText))

	TutorialManager:addTutorialState("ReadyToCreateSpinoff");
	TutorialManager:removeTutorialState("ReadyToFinishFirstRealWork");
end

function RunTutorial_OnReadyToCreateFirstSpinoff()
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_OnReadyToCreateFirstSpinoff"), ContinueText))
end

function RunTutorial_OnCreateFirstSpinoff()
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_OnCreateFirstSpinoff_Step1"), ContinueText))
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_OnCreateFirstSpinoff_Step2"), ContinueText))

	-- конец цепочки
	TutorialManager:removeTutorialState("ReadyToCreateSpinoff");
end

function RunTutorial_FirstInvestigationStarted()
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_FirstInvestigationStarted"), ContinueText))

	TutorialManager:addTutorialState("WaitForCatchingFirstInvestigator");
	TutorialManager:removeTutorialState("WaitForFirstInvestigator");
end

function RunTutorial_FirstInvestigationCatched()
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_FirstInvestigationCatched"), ContinueText))

	TutorialManager:addTutorialState("WaitForUncatchedInvestigator");
	TutorialManager:removeTutorialState("WaitForCatchingFirstInvestigator");
end

function RunTutorial_FirstUncatchedInvestigator()
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_FirstUncatchedInvestigator"), ContinueText))

	TutorialManager:addTutorialState("WaitForCatchUncatchedInvestigator");
	TutorialManager:removeTutorialState("WaitForUncatchedInvestigator");
end

function RunTutorial_FirstUncatchedInvestigatorCatched()
	TutorialManager:addTutorial(Tutorial(GetLocalizedString("Tutorial_FirstUncatchedInvestigatorCatched"), ContinueText, "TutorialsLastTutorial"))

	-- конец цепочки
	TutorialManager:removeTutorialState("WaitForCatchUncatchedInvestigator");
end

-- продолжить туториал после перезапуска игры (для особых случаев, которые не разрешаются простой загрузкой состояния)
function ContinueTutorial()
	local tutorialState = TutorialManager:getTutorialState()
	if tutorialState == "FirstCell" then
		RunTutorial_AfterCreatingFirstCell()
	end
end

function TutorialsLastTutorial()
end
