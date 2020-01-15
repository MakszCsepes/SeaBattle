--
-- Created by IntelliJ IDEA.
-- User: max
-- Date: 28/12/2019
-- Time: 19:23
-- To change this template use File | Settings | File Templates.
---------------------------------------------------
LEFT_SIDE = 1
RIGHT_SIDE = 2
UP_SIDE = 3
DOWN_SIDE = 4

hit_side = LEFT_SIDE

n = 10
m = 10

list_of_coords = {}
list_of_used_coords = {}

NO_HIT_HERE = 0
WAS_MISHIT = 1
WAS_HIT = 2
WAS_KILL = 3

last_i = nil
last_j = nil
----------------------------------------------------
function change_hit_side(stateMap)
    if stateMap['last_i']["last_j"] == WAS_HIT then
        if hit_side == LEFT_SIDE then
            hit_side = RIGHT_SIDE
        elseif hit_side == RIGHT_SIDE then
            hit_side = UP_SIDE
        elseif hit_side == UP_SIDE then
            hit_side = DOWN_SIDE
        elseif hit_side == DOWN_SIDE then
            hit_side = LEFT_SIDE
        end
    end
end

function if_last_is_no_hit(stateMap)
    if last_i == nil and last_j == nil or stateMap[last_i][last_j] == NO_HIT then
        return true
    end

    return false
end
math.randomseed(os.time())
function get_rand_coordinate()
    local i = math.random(n - 1)
    local j = math.random(m - 1)

    return {i = i, j = j}
end
function remember_last_coords(i, j)
    last_i = i
    last_j = j
end

function get_coords(stateMap)
    local new_coords = {}

    -- if this is the first AI step then last_i and last_j will have nil value
    -- so it should get random coordinates
    if last_i == nil or last_j == nil then
        new_coords = get_rand_coordinate()
        return new_coords
    end

    if stateMap['last_i']['last_j'] == WAS_MISHIT or stateMap['last_i']['last_j'] == WAS_KILL then
        new_coords = get_rand_coordinate()
    elseif stateMap['last_i']['last_j'] == WAS_HIT then
        if hit_side == LEFT_SIDE then
            new_coords = {i = last_i, j = last_j - 1 }
        elseif hit_side == RIGHT_SIDE then
            new_coords = {i = last_i, j = last_j + 1 }
        elseif hit_side == UP_SIDE then
            new_coords = {i = last_i - 1, j = last_j }
        elseif hit_side == DOWN_SIDE then
            new_coords = {i = last_i + 1, j = last_j }
        end
    end

    last_i = new_coords['i']
    last_j = new_coords['j']
    return new_coords
end

printMessage("You can call C++ functions from Lua!")