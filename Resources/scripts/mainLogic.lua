function StartGame()
	if World:isFirstLaunch() then
		FirstLaunch()
	end
end

function FirstLaunch()
	RunTutorialWelcome()
end
