for i = 0, 2 do
    print(i)
end

function foo(a, b, c)
    local sum = a + b
    return sum, c
end

r1, r2 = foo(1, "122", "Hello")
print(r1, r2)

local t1 = {}
local t2 = { x = 1, ["hello"] = "world" }
t1["string"] = "nihao"
t1[1] = 100
t1["table"] = t2

for k, v in pairs(t1) do
    print(k, "==>", v)
end

print(t1.string)

print("<========================================>")

local function create(name, id)
    local obj = { name = name, id = id }
    function obj:SetName(name)
        self.name = name
    end

    function obj:GetName()
        return self.name
    end

    return obj
end

local c = create("sam", 120)
for k, v in pairs(c) do
    print(k, "===>", v)
end

print("getname: ", c:GetName())