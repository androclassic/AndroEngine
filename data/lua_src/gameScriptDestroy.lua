require "data.lua_src.game_entity"

GameManager:DestroyAll()

for key, entity in pairs(game_objects) do
	DestroyGameObject(entity)
end