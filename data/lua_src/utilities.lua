
function shallowcopy(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in pairs(orig) do
            copy[orig_key] = orig_value
        end
    else -- number, string, boolean, etc
        copy = orig
    end
    return copy
end

function LUA_ERROR( msg )
	Print_ERROR( tostring(msg))
end



-- vector3
local Vector3MT = {} 

setmetatable(Vector3MT,
{
    __index = Vector3MT;
    __add = function(a,b)
                return Vector3(a.x+b.x,a.y+b.y,a.z+b.z)
            end;
    __tostring = function(a) 
                    return "("..a.x..','..a.y..','..a.z..")"
                end;
}
); --this metatable defines the overloads for tables with this metatable
function Vector3(x,y,z) --Vector3MT is implicitly assigned to a variable self
    return setmetatable({x=x or 0,y=y or 0,z=z or 0},getmetatable(Vector3MT)); --create a new table and give it the metatable of Vector3
end 

