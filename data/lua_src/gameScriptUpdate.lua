require "data.lua_src.game_entity"

if not timestep then
	timestep = 0
	scale = 0
end

timestep = timestep + 0.001

local angle = 2 * 3.14 /  #game_objects;
local radius =  15;


for  i = 1, #game_objects do
	local x = radius * math.cos(angle * (i + timestep))
	local y = radius * math.sin(angle * (i + timestep))
	game_objects[i]:SetPosition(x ,-10 , y)
end

GameManager:game_loop()
