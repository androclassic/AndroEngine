
GameManager = {}

GameManager.game_entites = {}


GameManager.game_loop = function( o )

	for key, entity in pairs(o.game_entites) do
		if entity.OnUpdate ~= nil then
			entity:OnUpdate()
		end	
	end
end


GameManager.DestroyAll = function( o )

	for key, entity in pairs(o.game_entites) do
	
		if( entity.OnDestroy ~= nil )then
			entity:OnDestroy()
		end
		
		DestroyGameObject(entity.__userdata)
	end
	o.game_entite = {}
	
end

function Entity( o )

	local model = o.Model
	local material = o.Material
	local name = o.Name
	
	local entity = CreateGameObject(material, model)
	
	local entity_lua = o
	entity_lua.__userdata = entity
	

	--anything that is not in entity_lua is in entity(which is userdata)
	setmetatable(entity_lua, getmetatable(entity)) 

	if GameManager.game_entites[o.Name] ~= nil then
		error("entity exists")
	end
	GameManager.game_entites[o.Name] = entity_lua
	
	if( entity_lua.OnInit ~= nil )then
		entity_lua:OnInit()
	end

end

 GameManager.GetEntity = function( o, name )
	return GameManager.game_entites[name]
end

GameManager.DestroyEntity = function( o, name )
	local entity =  GameManager.game_entites[name]
	
	if( entity.OnDestroy ~= nil )then
		entity:OnDestroy()
	end
	
	DestroyGameObject(entity.__userdata)
	GameManager.game_entites[name] = nil

end
