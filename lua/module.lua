--- 迭代
-- @param array
--
local function enum(array)
    local index = 1
    return function()
        local ret = array[index]
        index = index + 1
        return ret
    end
end

--- enum函数返回一个匿名的迭代函数, for语句每次调用该迭代函数都得到一个值(通过element变量引用),
--- 若该值为nil,则for循环结束
local function foreach(array, action)
    for element in enum(array) do
        action(element)
    end
end

foreach({ 1, 2, 3 }, print)


--- 协程
--- 创建协程
--- 通过 coroutine.create() 可以创建一个协程, 该函数接收一个函数类型的参数作为协程的执行体, 返回一个协程对象
---
--- 启动协程
--- 通过 coroutine.resume() 可以启动一个协程或者继续一个挂起的协程. 该函数接收一个协程对象及其他需要传递给该
--- 协程的参数. 协程可以通过协程函数的参数或者coroutine,yield()调用的返回值来获取这些参数.
--- 当协程初次执行时, resume传递的参数 "通过协程函数的参数" 传递给协程, 协程从协程的函数开始执行;
--- 当协程由挂起转为执行时, resume传递的参数 "以yield调用返回值的形式" 传递给线程, 协程从yield调用后继续执行;
---
--- 协程放弃调度
--- 协程调用 coroutine.yeild() 暂停自己的执行, 并把执行权返回给启动/继续它的协程; 协程还可利用yield返回一些
--- 值给后者, 这些值以resume调用的返回值的形式返回.
local function producer()
    return coroutine.create(function(salt)
        local t = { 1, 2, 3 }
        for i = 1, #t do
            salt = coroutine.yield(t[i] + salt)
        end
    end)
end

function consumer(prod)
    local salt = 10
    while true do
        local running, product = coroutine.resume(prod, salt)
        salt = salt * salt
        if running then
            print(product or "END!")
        else
            break
        end
    end
end

consumer(producer())
