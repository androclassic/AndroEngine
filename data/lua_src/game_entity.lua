require "data.lua_src.utilities"


GameManager = {}

GameManager.game_entites = {}
GameManager.classsystem = {}


-- TODO
function Entity( class )
	GameManager.classsystem[class.Name] =  class
end


GameManager.game_loop = function( o )

	for key, entities in pairs(o.game_entites) do
		for _k,entity in pairs(entities) do
			if entity.OnUpdate ~= nil then
				entity:OnUpdate()
			end	
		end
	end
end

GameManager.DestroyAll = function( o )

	for key, entities in pairs(o.game_entites) do
		for _k,entity in pairs(entities) do
			if( entity.OnDestroy ~= nil )then
				entity:OnDestroy()
			end
			DestroyGameObject(entity.__userdata)
		end
	end
	o.game_entite = {}
	
end

GameManager.CreateEntity= function (o, name )

	local class = GameManager.classsystem[name]

	local model = class.Model
	local material = class.Material
	local name = class.Name
	
	local entity = CreateGameObject(material, model)
	
	local entity_lua = shallowcopy(class)
	entity_lua.__userdata = entity
	

	--anything that is not in entity_lua is in entity(which is userdata)
	setmetatable(entity_lua, getmetatable(entity)) 

	if GameManager.game_entites[class.Name] == nil then
		 GameManager.game_entites[class.Name] = {}
	end
	table.insert(GameManager.game_entites[class.Name],entity_lua)
	
	if( entity_lua.OnInit ~= nil )then
		entity_lua:OnInit()
	end

	return entity_lua
end

 GameManager.GetEntities = function( o, name )
	return GameManager.game_entites[name]
end

GameManager.DestroyEntities = function( o, name )
	local entities =  GameManager.game_entites[name]
	
	for _k,entity in pairs(entities) do
		if( entity.OnDestroy ~= nil )then
			entity:OnDestroy()
		end
		
		DestroyGameObject(entity.__userdata)
		GameManager.game_entites[name] = nil
	end
end
