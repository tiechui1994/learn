--- 函数闭包
-- @param second
--
function countdown(second)
    local ms = second * 1000
    local function count()
        ms = ms - 1
        return ms
    end

    return count
end


local timer = countdown(1)
for i = 1, 3 do
    print(timer())
end

--- 基于对象的实现方式
-- @param name
-- @param id
--
local function create(name, id)
    local data = { name = name, id = id } -- data为obj.SetName,obj.GetName,obj.SetId,obj.GetId的Upvalue
    local obj = {} --把需要隐藏的成员放在一张表里,把该表作为成员函数的upvalue
    function obj.SetName(name)
        data.name = name
    end

    function obj.GetName()
        return data.name
    end

    function obj.SetId(id)
        data.id = id
    end

    function obj.GetId()
        return data.id
    end

    return obj
end

local c = create("tom", 12)
print(c:GetId())

--- 元表
--- 元表本身只是一个普通的表, 通过特定的方法(比如setmetatable)设置到某个对象上, 进而影响这个对象的行为;
--- 一个对象有哪些行为受到元表影响以及这些行为按照何种方式受到影响是受Lua语言约束的.
--- 比如在前面的代码里, 两个表对象的加法运算, 如果没有元表的干预, 就是一种错误;
--- 但是Lua规定了元表可以“重载”对象的加法运算符,因此若把定义了加法运算的元表设置到那两个表上,
--- 它们就可以做加法了. 元表是Lua最关键的概念之一,内容也很丰富, 请参考Lua文档了解详情.

--- 如果把表比作对象,元表就是可以改变对象行为的“元”对象
print('-------------- 元表 __index -------------------')
local t = {}
local m = { a = "and", b = "Li Lei", c = "Han Meimei" }
setmetatable(t, { __index = m }) -- 表{ __index=m }作为表t的元表

for k, v in pairs(t) do
    print("有值吗??")
    print(k, "==>", v)
end

print(t.b, t.a, t.c)

print('--------------- 元表__add -------------')
local function add(t1, t2)
    assert(#t1 == #t2)
    local length = #t1
    for i = 1, length do
        t1[i] = t1[i] + t2[i]
    end
    return t1
end

t1 = setmetatable({ 1, 2, 3 }, { __add = add })
t2 = setmetatable({ 10, 20, 30 }, { __add = add })

for k, v in pairs(t1) do
    print(k, "==>", v)
end

for k, v in pairs(t2) do
    print(k, "==>", v)
end

print("------------------------------")

t1 = t1 + t2
for i = 1, #t1 do
    print(t1[i])
end


--- 基于原型的继承
--- prototype模式一个对象既是一个普通的对象,同时也可以作为创建其他对象的原型的对象(即类对象, class object)
--- 动态的改变原型对象的属性就可以动态的影响所有基于此原型的对象; 另外, 基于一个原型被创建出来的对象可以重载任何属于这个原型对象
--- 的方法, 属性而不影响原型对象; 同时, 基于原型被创建出来的对象还可以作为原型来创建其他对象.
local Robot = { name = "Sam", id = 001 }
function Robot:New(extension)
    local t = setmetatable(extension or {}, self)
    self.__index = self
    return t
end

function Robot:SetName(name)
    self.name = name
end

function Robot:GetName()
    return self.name
end

robot = Robot:New()
print("robot name:", robot:GetName())

local FootballRobot = Robot:New({ position = "right back" })
function FootballRobot:SetPosition(p)
    self.position = p
end

function FootballRobot:GetPosition()
    return self.position
end

fr = FootballRobot:New()
print("fr's position:", fr:GetPosition())
print("fr's name:", fr:GetName())

print('===========================================')

fr:SetName("Bob")
print("fr's name:", fr:GetName())
print("robot's name:", robot:GetName())