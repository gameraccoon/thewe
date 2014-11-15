local ContinueText = "Продолжить"

function RunTutorial_Welcome()
	-- ToDo: make localizations
	local text1 = GetLocalizedString("Tutorial_Welcome_Step1")
		-- "Добро пожаловать в центр управления\n"..
		-- "Вашей тайной организацией!"

	Log:log(text1)

	local text2 =
		"Выберите город в котором сделаете\n" ..
		"свой первый шаг."

	World:addTutorial(Tutorial(text1, ContinueText))
	World:addTutorial(Tutorial(text2, ContinueText))

	World:setTutorialState("FirstCell")
end

function RunTutorial_AfterCreatingFirstCell()
	local text =
		"Поздравляю! Но в ячейке пока\n"..
		"недостаточно сотрудников для\n"..
		"полноценного функционирования.\n"..
		"Самое время начать вербовку.\n\n"..
		"Нажмите на ячейку и зайдите в\n"..
		"меню заданий."

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
		"Хорошо. Ячейке требуется некоторое время\n"..
		"для отсеивания кандидатов и их вербовки.\n\n"..
		"Этот процесс может занимать различное\n"..
		"время в зависимости от уровня ячейки и\n"..
		"напряжённости обстановки."

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("WaitingForFinishFirstTask");
end

function RunTutorial_AfterFirstTaskFinished()
	local text =
		"Ячейка завершила набор агентов!\n\n"..
		"Теперь в ней достаточно людей,\n"..
		"чтобы взяться за первое серьёзное\n"..
		"задание.\n\n"..
		"Откройте меню заданий."

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("ReadyToFirstRealWork");
end

function RunTutorial_StartingFirstRealWork()
	local text =
		"Бывают ситуации когда время важнее денег\n"..
		"Тогда можно используя подкуп определенных\n"..
		"людей мгновенно завершить текущее задание\n"..
		"Для этого потребуются черные кейсы."
	-- пока отключаем туториал про бустеры
	--World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("ReadyToFinishFirstRealWork");
end

function RunTutorial_AfterRealWorkDone()
	local text =
		"Сложно укрепить свою власть,\n"..
		"контроллируя всего один город.\n"..
		"Вы можете создавать новые ячейки\n"..
		"в других городах, развивая таким\n"..
		"образом свою тайную организацию.\n\n"..
		"Выберите свою ячейку и откройте\n"..
		"меню создания дочерних ячеек."

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("ReadyToCreateSpinoff");
end

function RunTutorial_OnReadyToCreateFirstSpinoff()
	local text =
		"Теперь выберите город, в котором\n"..
		"Вы хотите расположить новую ячейку."

	World:addTutorial(Tutorial(text, ContinueText))
end

function RunTutorial_OnCreateFirstSpinoff()
	local text1 =
		"Поздравляю с основанием дочерней ячейки.\n\n"..
		"После создания она будет функционировать\n"..
		"как полценный член вашей сети."

	local text2 =
		"Создавая и развивая ячейки, Вы\n"..
		"увеличивете своё влияние в стране.\n\n"..
		"Шкала внизу экрана показывает ваш\n"..
		"текущий уровень влияния\n\n"..
		"Теперь попробуйте взять под свой\n"..
		"контроль всю страну."

	World:addTutorial(Tutorial(text1, ContinueText))
	World:addTutorial(Tutorial(text2, ContinueText))

	World:setTutorialState("WaitForFirstInvestigator");
end

function RunTutorial_FirstInvestigationStarted()
	local text =
		"Наши ребята облажались и теперь\n"..
		"спецслужбы начали расследование.\n\n"..
		"Мы должны замести следы -\n"..
		"Нажмите на красную кнопку,\n"..
		"чтобы уничтожить ячейку."

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("WaitForCatchingFirstInvestigator");
end

function RunTutorial_FirstInvestigationCatched()
	local text =
		"Если не уничтожить ячейку, то до неё\n"..
		"всё равно доберутся и, будь уверен, расколят.\n\n"..
		"Это небольшая жертва ради нашей цели."

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("WaitForUncatchedInvestigator");
end

function RunTutorial_FirstUncatchedInvestigator()
	local text =
		"Спецслужбы добрались до наших парней\n"..
		"и уже раскалывают их.\n\n"..
		"Мы должны зачистить ячейки о которых\n"..
		"знали эти ребята, чтобы остановить\n"..
		"расследование и защитить остальную сеть."

	World:addTutorial(Tutorial(text, ContinueText))

	World:setTutorialState("WaitForCatchUncatchedInvestigator");
end

function RunTutorial_FirstUncatchedInvestigatorCatched()
	local text =
		"Расследование спецслужб точно\n"..
		"зайдёт в тупик без новых улик.\n\n"..
		"У нас могут остатся ячейки,\n"..
		"с которыми мы потеряли связь.\n\n"..
		"Самое вермя проверить это и изменить связи\n"

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
