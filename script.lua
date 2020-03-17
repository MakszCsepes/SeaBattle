--
-- Created by IntelliJ IDEA.
-- User: max
-- Date: 28/12/2019
-- Time: 19:23
-- To change this template use File | Settings | File Templates.
---------------------------------------------------
script_version = "AI Engine version: 1.0.1"

LEFT_SIDE = 1
RIGHT_SIDE = 2
UP_SIDE = 3
DOWN_SIDE = 4

hit_side = LEFT_SIDE

MAP_HEIGHT = 10
MAP_WIDTH = 10

list_of_possible_coords = {}

NO_HIT_HERE = 0
WAS_MISHIT = 1
WAS_HIT = 2
WAS_KILL = 3

first_i = nil
first_j = nil

last_i = nil
last_j = nil
----------------------------------------------------
function get_possible_coords_list(stateMap)
    local list = {}
    local index = 0
    for I = 0, MAP_HEIGHT-1, 1 do
        for J = 0, MAP_WIDTH-1, 1 do
            if stateMap[I][J] == NO_HIT_HERE then
                list[index] = { i = I, j = J}
            else
                list[index] = nil
            end
            index = index + 1
        end
    end

    return list;
end
function remember_last_coords(i, j)
    last_i = i
    last_j = j
end
math.randomseed(os.time())
function get_rand_coordinate()
    local index = math.random(#list_of_possible_coords)
    printMessage(index)

    return {i = list_of_possible_coords[index]['i'], j = list_of_possible_coords[index]['j']}
end
function get_coords(stateMap)
    list_of_possible_coords = get_possible_coords_list(stateMap)

    local new_coords = {}

    -- if this is the first AI step then last_i and last_j will have nil value
    -- so func should return random coordinates
    if last_i == nil or last_j == nil then
        new_coords = get_rand_coordinate()
    else
        if stateMap[last_i][last_j] == WAS_MISHIT or stateMap[last_i][last_j] == WAS_KILL then
            printMessage("was MISHIT")
            new_coords = get_rand_coordinate()
            first_i = new_coords['i']
            first_j = new_coords['j']
        elseif stateMap[last_i][last_j] == WAS_HIT then
            printMessage("was HIT")
            if hit_side == LEFT_SIDE then
                if (last_j == 0) then
                    if (last_j ~= first_j) then
                        hit_side = RIGHT_SIDE
                        new_coords = {i = last_i, j = last_j + 1}
                    end
                else
                    new_coords = {i = last_i , j = last_j - 1}
                end
            elseif hit_side == RIGHT_SIDE then
                if (last_j == MAP_WIDTH - 1) then
                    if(last_j ~= first_j) then
                        hit_side = LEFT_SIDE
                        new_coords = {i = last_i, j = first_j - 1}
                    else
                        new_coords = {i = last_i, j = last_j - 1}
                    end
                else
                    new_coords = {i = last_i, j = last_j + 1}
                end
            elseif hit_side == UP_SIDE then
                new_coords = {i = last_i - 1, j = last_j}
            elseif hit_side == DOWN_SIDE then
                new_coords = {i = last_i + 1, j = last_j}
            end
        end
    end

    remember_last_coords(new_coords['i'], new_coords['j'])

    return new_coords
end

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

printMessage("AI Engine version: 1.0.1\n")