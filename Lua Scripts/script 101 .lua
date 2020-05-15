script_version = "AI Engine version: 1.0.1"

math.randomseed(os.time())

LEFT_SIDE = 1
RIGHT_SIDE = 2
UP_SIDE = 3
DOWN_SIDE = 4

hit_side = LEFT_SIDE

MAP_HEIGHT = 10
MAP_WIDTH = 10

list_of_possible_coords = {}

EMPTY_CELL = 0
MISHIT_CELL = 1
PALUBA_CELL = 2
HIT_PALUBA_CELL = 3
KILLED_PALUBA_CELL = 4

first_i = nil
first_j = nil

last_i = nil
last_j = nil
----------------------------------------------------
function get_possible_coords_list(stateMap)
    local list = {}
    local index = 0

    for i = 0, MAP_HEIGHT-1, 1 do
        for j = 0, MAP_WIDTH-1, 1 do
            if stateMap[i][j] == EMPTY_CELL then
                list[index] = { y = i, x = j}
            elseif stateMap[i][j] == PALUBA_CELL then
                list[index] = { y = i, x = j }
            else
                index = index - 1;
            end
            index = index + 1;
        end
    end

    return list;
end
function remember_last_coords(i, j)
    last_i = i
    last_j = j
end

function get_rand_coordinate()
    local index = math.random(#list_of_possible_coords)

    return {i = list_of_possible_coords[index]['y'], j = list_of_possible_coords[index]['x']}
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

function get_coords(stateMap)
    list_of_possible_coords = get_possible_coords_list(stateMap)

    local new_coords = {}

    -- if this is the first AI step then last_i and last_j will have nil value
    -- so func should return random coordinates
    if last_i == nil or last_j == nil then
        new_coords = get_rand_coordinate()
    else
        if stateMap[last_i][last_j] == MISHIT_CELL or stateMap[last_i][last_j] == KILLED_PALUBA_CELL then
            new_coords = get_rand_coordinate()
            first_i = new_coords['i']
            first_j = new_coords['j']
        elseif stateMap[last_i][last_j] == HIT_PALUBA_CELL then
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

    return new_coords;
end














