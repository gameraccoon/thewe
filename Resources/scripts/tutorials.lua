local ContinueText = GetLocalizedString("Tutorial_BtnContinue")

function RunTutorial_Welcome()
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_Welcome_Step1"), ContinueText))
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_Welcome_Step2"), ContinueText))

	-- запускаем цепочки туториалов
	World:addTutorialState("FirstCell")
	World:addTutorialState("WaitForFirstInvestigator");
end

function RunTutorial_AfterCreatingFirstCell()
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_AfterCreatingFirstCell"), ContinueText))

	World:addTutorialState("StartFirstTask");
	World:removeTutorialState("FirstCell");
end

function RunTutorial_BeforeStartFirstTask()
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_BeforeStartFirstTask"), ContinueText))

	World:addTutorialState("WaitingForStartFirstTask");
	World:removeTutorialState("StartFirstTask");
end

function RunTutorial_StartingFirstTask()
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_StartingFirstTask"), ContinueText))

	World:addTutorialState("WaitingForFinishFirstTask");
	World:removeTutorialState("WaitingForStartFirstTask");
end

function RunTutorial_AfterFirstTaskFinished()
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_AfterFirstTaskFinished"), ContinueText))

	World:addTutorialState("ReadyToFirstRealWork");
	World:removeTutorialState("WaitingForFinishFirstTask");
end

function RunTutorial_StartingFirstRealWork()
	-- пока отключаем туториал про бустеры
	--World:addTutorial(Tutorial(GetLocalizedString("Tutorial_StartingFirstRealWork"), ContinueText))

	World:addTutorialState("ReadyToFinishFirstRealWork");
	World:removeTutorialState("ReadyToFirstRealWork");
end

function RunTutorial_AfterRealWorkDone()
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_AfterRealWorkDone"), ContinueText))

	World:addTutorialState("ReadyToCreateSpinoff");
	World:removeTutorialState("ReadyToFinishFirstRealWork");
end

function RunTutorial_OnReadyToCreateFirstSpinoff()
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_OnReadyToCreateFirstSpinoff"), ContinueText))
end

function RunTutorial_OnCreateFirstSpinoff()
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_OnCreateFirstSpinoff_Step1"), ContinueText))
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_OnCreateFirstSpinoff_Step2"), ContinueText))

	-- конец цепочки
	World:removeTutorialState("ReadyToCreateSpinoff");
end

function RunTutorial_FirstInvestigationStarted()
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_FirstInvestigationStarted"), ContinueText))

	World:addTutorialState("WaitForCatchingFirstInvestigator");
	World:removeTutorialState("WaitForFirstInvestigator");
end

function RunTutorial_FirstInvestigationCatched()
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_FirstInvestigationCatched"), ContinueText))

	World:addTutorialState("WaitForUncatchedInvestigator");
	World:removeTutorialState("WaitForCatchingFirstInvestigator");
end

function RunTutorial_FirstUncatchedInvestigator()
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_FirstUncatchedInvestigator"), ContinueText))

	World:addTutorialState("WaitForCatchUncatchedInvestigator");
	World:removeTutorialState("WaitForUncatchedInvestigator");
end

function RunTutorial_FirstUncatchedInvestigatorCatched()
	World:addTutorial(Tutorial(GetLocalizedString("Tutorial_FirstUncatchedInvestigatorCatched"), ContinueText, "TutorialsLastTutorial"))

	-- конец цепочки
	World:removeTutorialState("WaitForCatchUncatchedInvestigator");
end

-- продолжить туториал после перезапуска игры (для особых случаев, которые не разрешаются простой загрузкой состояния)
function ContinueTutorial()
	local tutorialState = World:getTutorialState()
	if tutorialState == "FirstCell" then
		RunTutorial_AfterCreatingFirstCell()
	end
end

function TutorialsLastTutorial()
end
