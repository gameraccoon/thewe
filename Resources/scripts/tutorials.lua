local ContinueText = "Продолжить"

function RunTutorial_Welcome()
	-- ToDo: make localizations
	local text1 =
		"Добро пожаловать в игру!\n"..
		"Это альфа-версия игры \"The We\""

	local text2 =
		"Для начала нажмите на любой город,\n"..
		"чтобы основать там свою первую ячейку"

	World:addTutorial(Tutorial(text1, ContinueText))
	World:addTutorial(Tutorial(text2, ContinueText))

	World:setTutorialState("FirstCell")
end

function RunTutorial_AfterCreatingFirstCell()
	local text =
		"Отлично!\n\n"..
		"Сейчас в ячейке совсем не много сотрудников.\n"..
		"Для действительно великих дел нам\n"..
		"понадобится больше людей.\n\n"..
		"Нажмите на ячейку и зайдите в меню заданий"

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("StartFirstTask");
end

function RunTutorial_BeforeStartFirstTask()
	local text =
		"Теперь выберите задачу вербовки"

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("WaitingForStartFirstTask");
end

function RunTutorial_StartingFirstTask()
	local text =
		"Хорошо. Сейчас мы должны подождать, пока\n"..
		"ячейка наберёт в свои ряды достойных\n"..
		"союзников\n\n"..
		"В зависимости от уровня ячейки и\n"..
		"напряжённости обстановки, этот процесс\n"..
		"может занимать разное время"

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("WaitingForFinishFirstTask");
end

function RunTutorial_AfterFirstTaskFinished()
	local text =
		"Ячейка завершила набор сотрудников!\n\n"..
		"Теперь у нас есть достаточно людей\n"..
		"чтобы отправить ячейку на первое серьёзное\n"..
		"задание"

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("ReadyToFirstRealWork");
end

function RunTutorial_StartingFirstRealWork()
	local text =
		"Тут надо сказать про бустеры"

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("ReadyToFinishFirstRealWork");
end

function RunTutorial_AfterRealWorkDone()
	local text =
		"Спинофф"

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("ReadyToCreateSpinoff");
end

function RunTutorial_OnReadyToCreateFirstSpinoff()
	local text =
		"Выберите город"

	World:addTutorial(Tutorial(text, ContinueText))
end

function RunTutorial_OnCreateFirstSpinoff()
	local text =
		"Создали новую ячейку"

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("WaitForFirstInvestigator");
end

function RunTutorial_FirstInvestigationStarted()
	local text =
		"Запущен инвестигейтор"

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("WaitForCatchingFirstInvestigator");
end

function RunTutorial_FirstInvestigationCatched()
	local text =
		"Закрыли инвестигейтора"

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("WaitForUncatchedInvestigator");
end

function RunTutorial_FirstUncatchedInvestigator()
	local text =
		"Профукали инвестигейтора"

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("WaitForCatchUncatchedInvestigator");
end

function RunTutorial_FirstUncatchedInvestigatorCatched()
	local text =
		"Говорим о том что нужно перелинковать сеть"

	World:addTutorial(Tutorial(text, ContinueText, "TutorialsLastTutorial"))
end

-- продолжить туториал после перезапуска игры (для особых случаев, которые не разрешатся простой загрузкой состояния)
function ContinueTutorial()
	local tutorialState = World:getTutorialState()
	if tutorialState == "FirstCell" then
		RunTutorial_AfterCreatingFirstCell()
	end
end

function TutorialsLastTutorial()
	World:setTutorialState("TutorialsEnd");
end
