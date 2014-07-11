
-- usage: (use instead of ant)
-- tolua++ "-L" "basic.lua" "-o" "LuaCocos2d.cpp" "Cocos2d.pkg"

_is_functions = _is_functions or {}
_to_functions = _to_functions or {}
_push_functions = _push_functions or {}
local CCObjectTypes = 
{
	"uidMaster",
	"uidEntity",
    "uidMurderer",
	"uidTarget",
    "uidCasualty",
	"uidGoodsID",
    "uidOperater",
	"uEntityId",
	"uidSrc",
	"uidMedicament",  
}

local toWrite = {}
local currentString = ''
local out
local WRITE, OUTPUT = write, output

function output(s)
    out = _OUTPUT
    output = OUTPUT -- restore
    output(s)
end

function write(a)
    if out == _OUTPUT then
        currentString = currentString .. a
        if string.sub(currentString,-1) == '\n'  then
            toWrite[#toWrite+1] = currentString
            currentString = ''
        end
    else
        WRITE(a)
    end
end

function post_output_hook(package)


    local result = table.concat(toWrite)
	--print(result);
    local function replace(pattern, replacement)
        local k = 0
        local nxt, currentString = 1, ''
		
        repeat
			
            local s, e = string.find(result, pattern, nxt, true)
            if e then
                currentString = currentString .. string.sub(result, nxt, s-1) .. replacement
                nxt = e + 1
                k = k + 1
            end
        until not e
        result = currentString..string.sub(result, nxt)
        if k == 0 then print('Pattern not replaced', pattern) end
    end

	
    local function replace1(pattern, replacement)
        local k = 0
        local nxt, currentString = 1, ''
        repeat
			
            local s, e = string.find(result, pattern, nxt, true)
            if e then
                currentString = currentString .. string.sub(result, nxt, s-1) .. replacement
                nxt = e + 1
                k = k + 1
            end
        until not e
        result = currentString..string.sub(result, nxt)
        if k == 0 then print('Pattern not replaced', pattern) end
    end

   
    replace([[ int tolua_ret = (int)  self->size();]],  [[ int tolua_ret = (int)  sizeof(*self);]])
	
	replace([[const _cstring]],  [[const char *]])
	replace([[_cstring]],  [[char *]])
	replace([[#include "../../Include/]],  [[#include "]])
	replace([[#include "../../../Include/]],  [[#include "]])
	replace([[#include "../../Common/]],  [[#include "]])
	replace([[#include "../../../Common/]],  [[#include "]]) 
	
    WRITE(result)
end
