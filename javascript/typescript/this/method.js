/**
 * js当中: this 永远指向最后调用它的那个对象.
 *
 * 改变this的指向方法:
 *    1. call, apply, bind 方法
 *    2. 使用 ES6 的箭头函数
 *    3. 在函数内部使用 _this = this
 *    4. new 实例化一个对象
 *
 * call: fun.call(thisObj, arg1, arg2,...argN) 定义：调用一个对象的一个方法,以另一个对象替换当前对象.
 *       说明：thisArg：在fun函数运行时指定的this值.需要注意的是,指定的this值并不一定是该函数执行时真正的this值,
 *            如果这个函数处于非严格模式下,则指定为 null 或 undefined 时会自动指向全局对象(浏览器中就是window对象),
 *            同时值为原始值(数字，字符串，布尔值)的 this 会指向该原始值的自动包装对象.
 *
 *            arg1 … argN为被调用方法的传参.
 *
 * apply: fun.apply(thisObj, argArray) 定义:应用某一对象的一个方法，用另一个对象替换当前对象.
 *       说明：thisArg：在fun函数运行时指定的this值.需要注意的是,指定的this值并不一定是该函数执行时真正的this值,
 *            如果这个函数处于非严格模式下,则指定为 null 或 undefined 时会自动指向全局对象(浏览器中就是window对象),
 *            同时值为原始值(数字，字符串，布尔值)的 this 会指向该原始值的自动包装对象.
 *
 *            argsArray：一个数组或者类数组对象,其中的数组元素将作为单独的参数传给 fun 函数.如果该参数的值为null 或 undefined,
 *            则表示不需要传入任何参数. 从ECMAScript 5 开始可以使用类数组对象.
 *
 *
 * bind: fun.bind(thisObj, arg1, arg2, ...argN) 定义: 应用某一对象的一个方法,用另一个对象替换当前对象. 创建一个的新的函数.
 *       说明：thisObj: 该参数也和call方法一样.
 *
 *            arg1 … argN: 该参数可传可不传.如果不传,可以在调用的时候再传. 如果传了,调用的时候则可以不传,调用的时候如果你还是传了,则不生效.
 *
 * 区别: ① call的arg传参需一个一个传, apply则直接传一个数组.
 *      ② call和apply直接执行函数, 而bind需要再一次调用.
 */
window.color = 'red';
document.color = 'yello';
var s = {color: 'blue'};


function callColor() {
  console.log('The  color is ' + this.color)
}

function say(value) {
  console.log(this.color + ' is ' + value)
}

callColor.call(); // 等价于color.call(window)
callColor.call(document);
callColor.call(s);
callColor();


console.log('--------------------');

say.call(window, 'window');
say.call(document, 'document');
say.call(s, 's');

console.log('-------------------');

callColor.apply(); // 等价于say.apply(window)
callColor.apply(document);
callColor.apply(s);

console.log('-------------------');

say.apply(window, ['window']);
say.apply(document, ['document']);
say.apply(s, ['s']);

console.log('-------------------');

callColor.bind()();  // 等价于 callColor.bind(window)();
callColor.bind(document)();
callColor.bind(s)();

console.log('-------------------');

say.bind()('window');
say.bind(document)('document');

/**
 * call, apply, bind 实际应用.
 *
 * 1. call 实现继承
 * 2. apply 数组追加
 * 3. apply 获取数组中的最大值和最小值
 * 4. bind 保存this变量
 */

// call 实现继承
function Animal(name) {
  this.name = name;
  this.showName = function () {
    console.log(this.name);
  }
}
function Cat(name) {
  Animal.call(this, name);
}
var cat = new Cat('Black Cat');
cat.showName();


// apply 数组追加
var array1 = [1, 2, 3, 5];
var array2 = ["xie", "li", "qun", "tsrot"];
Array.prototype.push.apply(array1, array2);
console.log(array1);

// apply 获取数组的最大值和最小值
var num = [1, 3, 5, 7, 2, -10, 11];
var maxNum = Math.max.apply(Math, num);
var minNum = Math.min.apply(Math, num);
console.log(maxNum);
console.log(minNum);

// bind 保存this变量
var foo = {
  bar: 1,
  eventBind: function () {
    var _this = this;
    $('.someClass').on('click', function (event) {
      console.log(_this.bar);
    });
  }
};
var foo = {
  bar: 1,
  eventBind: function () {
    $('.someClass').on('click', function (event) {
      console.log(this.bar);
    }.bind(this));
  }
};
