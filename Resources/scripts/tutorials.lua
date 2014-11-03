local ContinueText = "Продолжить"

function RunTutorial_Welcome()
	-- ToDo: make localizations
	local text1 =
		"Добро пожаловать в систему $Username$!\n"..
		"На данный момент состояние классифицируется \n"..
		"как альфа-версия \"The We\"."

	local text2 =
		"Выберите город в котором сделаете\n" ..
		"свой первый шаг. На данный момент\n"..
		"выбор города никак не влияет на сложность."

	World:addTutorial(Tutorial(text1, ContinueText))
	World:addTutorial(Tutorial(text2, ContinueText))

	World:setTutorialState("FirstCell")
end

function RunTutorial_AfterCreatingFirstCell()
	local text =
		"Поздравляю!\n\n"..
		"Но в ячейке недостаточно сотрудников для\n"..
		"полноценного функционирования.\n"..
		"Рекоммендую начать вербовку.\n\n"..
		"Нажмите на ячейку и зайдите в меню заданий."

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("StartFirstTask");
end

function RunTutorial_BeforeStartFirstTask()
	local text =
		"Теперь выберите задачу вербовки."

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("WaitingForStartFirstTask");
end

function RunTutorial_StartingFirstTask()
	local text =
		"Хорошо. Ячейке требуется некоторое время,\n"..
		"для отсеивания кандидатов и их вербовки,\n"..
		"рекоммендуется подождать.\n\n"..
		"В зависимости от уровня ячейки и\n"..
		"напряжённости обстановки, этот процесс\n"..
		"может занимать различное время."

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("WaitingForFinishFirstTask");
end

function RunTutorial_AfterFirstTaskFinished()
	local text =
		"Ячейка завершила набор агентов!\n\n"..
		"Теперь у нее достаточно людей\n"..
		"чтобы взяться за первое серьёзное\n"..
		"задание. Откройте меню заданий."

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("ReadyToFirstRealWork");
end

function RunTutorial_StartingFirstRealWork()
	local text =
		"Бывают ситуации когда время важнее денег\n"..
		"Тогда можно используя подкуп определенных\n"..
		"людей. Мгновенно завершить текущее задание\n"..
		"ячейки.\n"..
		"Это для этого потребуются черные кейсы."

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("ReadyToFinishFirstRealWork");
end

function RunTutorial_AfterRealWorkDone()
	local text =
		"Сложно укрепить свою власть посредством\n"..
		"такого малого числа людей.\n"..
		"Рекоммендую создать дочернюю ячейку.\n"..
		"Откройте меню ячейки и выберите пункт \"Spinoff\" \n"

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("ReadyToCreateSpinoff");
end

function RunTutorial_OnReadyToCreateFirstSpinoff()
	local text =
		"Теперь выберите город,\n"..
		"в котором вы хотите расположить ее."

	World:addTutorial(Tutorial(text, ContinueText))
end

function RunTutorial_OnCreateFirstSpinoff()
	local text =
		"Поздравляю с созданием дочерней ячейки.\n"..
		"После создания она будет функционировать как\n"..
		"полценный член вашей сети."

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
		"Для того чтобы оставаться в тени, надо быть\n"..
		"на шаг впереди любого из ваших преследователей.\n"..
		"Не забывайте время от времени менять связи между\n"..
		"Ячейками. Попробуйте сейчас это сделать.\n"

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
