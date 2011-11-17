.. index::
   single: Event Dispatcher

継承無しでクラスを拡張する方法
===============================================

複数のクラスにメソッドを追加ようにするために、次のように拡張したいクラス内でマジックメソッドの ``__call()`` メソッドを定義することができます。
To allow multiple classes to add methods to another one, you can define the
magic ``__call()`` method in the class you want to be extended like this:

.. code-block:: php

    class Foo
    {
        // ...

        public function __call($method, $arguments)
        {
            // 'foo.method_is_not_found' というイベントを作成してください
            $event = new HandleUndefinedMethodEvent($this, $method, $arguments);
            $this->dispatcher->dispatch($this, 'foo.method_is_not_found', $event);

            // イベントを実行するリスナがときは、メソッドが存在していません
            if (!$event->isProcessed()) {
                throw new \Exception(sprintf('Call to undefined method %s::%s.', get_class($this), $method));
            }

            // return the listener returned value
            return $event->getReturnValue();
        }
    }

上記のコードでは、特別な ``HandleUndefinedMethodEvent`` イベントを使用しているので、作成する必要があります。このクラスは、ジェネリックなクラスで、クラス拡張のこのパターンを使用する際に毎回再利用することができます。必要がある度に、再利用されるジェネリックなクラスです。

.. code-block:: php

    use Symfony\Component\EventDispatcher\Event;

    class HandleUndefinedMethodEvent extends Event
    {
        protected $subject;
        protected $method;
        protected $arguments;
        protected $returnValue;
        protected $isProcessed = false;

        public function __construct($subject, $method, $arguments)
        {
            $this->subject = $subject;
            $this->method = $method;
            $this->arguments = $arguments;
        }

        public function getSubject()
        {
            return $this->subject;
        }

        public function getMethod()
        {
            return $this->method;
        }

        public function getArguments()
        {
            return $this->arguments;
        }

        /**
         * 返す値をセットして、通知された他のリスナを止めます
         */
        public function setReturnValue($val)
        {
            $this->returnValue = $val;
            $this->isProcessed = true;
            $this->stopPropagation();
        }

        public function getReturnValue($val)
        {
            return $this->returnValue;
        }

        public function isProcessed()
        {
            return $this->isProcessed;
        }
    }

次に、 ``foo.method_is_not_found`` イベントをリッスンして、 ``bar()`` メソッドを *追加* します。

.. code-block:: php

    class Bar
    {
        public function onFooMethodIsNotFound(HandleUndefinedMethodEvent $event)
        {
            // ``bar`` メソッドへの呼び出しのみに応答します
            if ('bar' != $event->getMethod()) {
                // allow another listener to take care of this unknown method
                return;
            }

            // 　サブジェクトオブジェクト(foo インスタンス)
            $foo = $event->getSubject();

            // bar メソッドの引数
            $arguments = $event->getArguments();

            // 何か実装する
            // ...

            // 返り値をセットする
            $event->setReturnValue($someValue);
        }
    }

最後に、 ``Bar`` のインスタンスを ``foo.method_is_not_found`` イベントに登録して ``bar`` メソッドを ``Foo`` クラスに新しく追加してください。

.. code-block:: php

    $bar = new Bar();
    $dispatcher->addListener('foo.method_is_not_found', $bar);

.. 2011/11/18 ganchiku 4021613d0c9a5a967fc50ed68dacebc06833bd50

