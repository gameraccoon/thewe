local ContinueText = GetLocalizedString("Tutorial_BtnContinue")

local TutorialManager = World:getTutorialManager()

function RunTutorial_Welcome()
	TutorialManager:addTutorial(Tutorial("Welcome", GetLocalizedString("Tutorial_Welcome_Step1"), ContinueText))
	TutorialManager:addTutorial(Tutorial("FirstCell", GetLocalizedString("Tutorial_Welcome_Step2")))

	-- запускаем цепочки туториалов
	TutorialManager:addTutorialState("FirstCell")
	TutorialManager:addTutorialState("WaitForFirstInvestigator");
end

function RunTutorial_AfterCreatingFirstCell()
	TutorialManager:addTutorial(Tutorial("AfterFirstCell", GetLocalizedString("Tutorial_AfterCreatingFirstCell")))

	TutorialManager:addTutorialState("StartFirstTask");
	TutorialManager:removeTutorialState("FirstCell");
end

function RunTutorial_BeforeStartFirstTask()
	TutorialManager:addTutorial(Tutorial("StartFirstTask", GetLocalizedString("Tutorial_BeforeStartFirstTask")))

	TutorialManager:addTutorialState("WaitingForFinishFirstTask");
	TutorialManager:removeTutorialState("StartFirstTask");
end

function RunTutorial_AfterFirstTaskFinished()
	TutorialManager:addTutorial(Tutorial("Textual", GetLocalizedString("Tutorial_AfterFirstTaskFinished"), ContinueText))

	TutorialManager:addTutorialState("ReadyToFinishFirstRealWork");
	TutorialManager:removeTutorialState("WaitingForFinishFirstTask");
end

function RunTutorial_AfterRealWorkDone()
	TutorialManager:addTutorial(Tutorial("SpinoffDrag", GetLocalizedString("Tutorial_AfterRealWorkDone")))

	TutorialManager:addTutorialState("ReadyToCreateSpinoff");
	TutorialManager:removeTutorialState("ReadyToFinishFirstRealWork");
end

function RunTutorial_OnCreateFirstSpinoff()
	TutorialManager:addTutorial(Tutorial("Textual", GetLocalizedString("Tutorial_OnCreateFirstSpinoff_Step1"), ContinueText))
	TutorialManager:addTutorial(Tutorial("SpinoffStep2", GetLocalizedString("Tutorial_OnCreateFirstSpinoff_Step2"), ContinueText))

	-- конец цепочки
	TutorialManager:removeTutorialState("ReadyToCreateSpinoff");
end

function RunTutorial_FirstInvestigationStarted()
	TutorialManager:addTutorial(Tutorial("InvestigationStarted", GetLocalizedString("Tutorial_FirstInvestigationStarted")))

	TutorialManager:addTutorialState("WaitForUncatchedInvestigator");
	TutorialManager:removeTutorialState("WaitForFirstInvestigator");
end

function RunTutorial_FirstUncatchedInvestigator()
	TutorialManager:addTutorial(Tutorial("InvestigatorUncatched", GetLocalizedString("Tutorial_FirstUncatchedInvestigator")))

	TutorialManager:addTutorialState("WaitForCatchUncatchedInvestigator");
	TutorialManager:removeTutorialState("WaitForUncatchedInvestigator");
end

function RunTutorial_FirstUncatchedInvestigatorCatched()
	TutorialManager:addTutorial(Tutorial("Textual", GetLocalizedString("Tutorial_FirstUncatchedInvestigatorCatched"), ContinueText))

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

