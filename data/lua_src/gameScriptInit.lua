
require "data.lua_src.effects.effects"

require "data.lua_src.objects"
require "data.lua_src.dwarf"

-- GameManager:CreateEntity("CubeNormal")
-- GameManager:CreateEntity("CubeDiffuse")
-- GameManager:CreateEntity("CubeSpecular")

for i=1, dwarfNumbers do
	GameManager:CreateEntity("Dwarf2")
end
	
