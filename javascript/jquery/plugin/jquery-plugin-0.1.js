// JQuery插件开发

// 方式一: $.extend(object) 扩展jQuery
$.extend({
  log: function (message) {
    var now = new Date(),
      y = now.getFullYear(),
      m = now.getMonth() + 1, //JavaScript中月分是从0开始的
      d = now.getDate(),
      h = now.getHours(),
      min = now.getMinutes(),
      s = now.getSeconds(),
      time = y + '/' + m + '/' + d + ' ' + h + ':' + min + ':' + s;
    console.log(time + ' My App: ' + message);
  }
});

// 方式二: $.fn[plugName] 向jQuery添加新的方法

/**
 * (function ($, window, document) {
 *   // 定义类,及其方法
 *   $.fn.pluginName = function () { // 插件调用的方法
 *      //code...
 *   }
 * })(jQuery, window, document);
 *
 *  说明: function (root, factory, plug) 是匿名方法
 *       (window, function($, plug){...}, plugName) 是匿名调用参数, 对应匿名方法的三个参数
 * (function (root, factory, plug) {
 *    return factory.call(root, root.jQuery, plug)
 * })(window, function($, plug) {
 *    // 定义类,及其方法
 *    $.fn[plug] = function () {  //插件调用的方法
 *       // code
 *    }
 * }, pluginName);
 */

// 面向过程的插件开发
// 匿名函数调用 (匿名函数)(匿名函数参数)
(function (root, factory, plug) {
  return factory.call(root, root.jQuery, plug);
})(window, function ($, plug) {
  // alert($);
  // alert(plug);
  $.fn[plug] = function (options) { // 插件添加的方法
    var __defaults__ = {  //默认参数
      color: 'red',
      fontSize: '18px'
    };
    var settings = $.extend({}, __defaults__, options); // 参数覆盖, 后面的会覆盖前面的参数值, 空对象可以保存默认的参数
    this.css({
      'color': settings.color,
      'fontSize': settings.fontSize
    });
    return this.each(function () {  // 支持链式调用
      $(this).append(' ' + $(this).attr('href'))
    })
  }
}, 'pluginName');


// 面向对象的插件开发

//定义Beautifier的构造函数, Beautifier为全局变量,它会被附到全局的window对象上
var Beautifier = function (ele, opt) {
  this.$element = ele;
  this.defaults = {
    'color': 'red',
    'fontSize': '12px',
    'textDecoration': 'none'
  };
  this.options = $.extend({}, this.defaults, opt)
};

//定义Beautifier的方法
Beautifier.prototype.beautify = function () {
  return this.$element.css(
    {
      'color': this.options.color,
      'fontSize': this.options.fontSize,
      'textDecoration': this.options.textDecoration
    }
  );
};

// 定义插件
(function (root, factory, plug) {
  return factory.call(root, root.jQuery, plug)
})(window, function ($, plug) {
  $.fn[plug] = function (options) {
    var beautifier = new Beautifier(this, options);
    return beautifier.beautify();
  }
}, 'beautify');

// 方式三: $.widget()应用jQuery UI的部件工厂方式创建