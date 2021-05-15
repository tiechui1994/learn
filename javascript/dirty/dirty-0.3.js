/**
 * 双向绑定 + 脏检查的优化版本
 */
window.onload = function () {
  var scope = {
    increaseCola: function () {
      this.cola++;
    },
    decreaseCola: function () {
      this.cola--;
    },
    increaseSprite: function () {
      this.sprite++;
    },
    decreaseSprite: function () {
      this.sprite--;
    },
    cola: 0,
    sprite: 0,
    price: 3
  };

  function Scope() {
    this.$$watchList = [];
  }

  Scope.prototype.$watch = function (name, getNewValue, listener) {
    var watch = {
      name: name,
      getNewValue: getNewValue,
      listener: listener || function () {
      }
    };

    this.$$watchList.push(watch);
  };

  Scope.prototype.$digest = function () {
    var dirty = true; //执行脏检查
    var checkTimes = 1; //脏检查的次数约束
    while (dirty) {
      dirty = false;
      var list = this.$$watchList;
      for (var i = 0; i < list.length; i++) {
        var watch = list[i];
        console.log(watch.name + ' is checked');
        var newValue = watch.getNewValue();
        var oldValue = watch.last;

        if (newValue !== oldValue) {
          watch.listener(newValue, oldValue); // 修改值
          dirty = true; // 再次检查修改后的值
        } else {
          scope[watch.name] = newValue; // 脏检查稳定赋值, 提供后面的渲染
        }

        watch.last = newValue; //记录上一次更改后的值
      }

      watch.last = newValue;
      checkTimes++;
      if (checkTimes > 10 && checkTimes) { // 脏检查次数的限制
        throw new Error('次数太多')
      }
    }
  };

  var $scope = new Scope();
  $scope.$watch('cola', function () {
    $scope.cola = scope.cola;
    return $scope[this.name]
  }, function (newValue, oldValue) {
    console.log('cola:          new: ' + newValue + '~~~~' + ' old: ' + oldValue);
  });

  $scope.$watch('sprite', function () {
    $scope.sprite = scope.sprite;
    return $scope[this.name]
  }, function (newValue, oldValue) {
    console.log('sprite:          new: ' + newValue + '~~~~' + ' old: ' + oldValue);
  });

  $scope.$watch('sum', function () {
    $scope.sum = scope.cola * scope.price + scope.sprite * scope.price; // 计算sum的值
    return $scope[this.name]
  }, function (newValue, oldValue) {
    console.log('sum:          new: ' + newValue + '~~~~' + ' old: ' + oldValue);
  });

  function bind() { //绑定操作, ng-click事件的绑定
    var list = document.querySelectorAll('[ng-click]');
    for (var i = 0; i < list.length; i++) {
      list[i].onclick = (function (index) {
        return function () {
          var func = this.getAttribute('ng-click'); //事件函数名称
          console.log('click event: ' + func);
          scope[func](); //执行事件函数,事件的发生导致数据的更改操作
          apply(); // 渲染数据
        }
      })(i);
    }
  }

  function apply() {
    $scope.$digest(); //数据脏检查
    var list = document.querySelectorAll('[ng-bind]');
    for (var i = 0; i < list.length; i++) {
      var bindData = list[i].getAttribute('ng-bind');
      console.log('属性:' + bindData + '为' + scope[bindData]);
      list[i].innerHTML = scope[bindData];
    }
  }

  bind(); // 绑定事件
  apply(); //首次渲染数据
};