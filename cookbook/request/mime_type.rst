.. index::
   single: Request; Add a request format and mime type

新しいリクエストのフォーマットとマイムタイプの登録方法
==================================================

全ての ``Request`` は "format"(例えば、 ``html`` や ``json``) を持っており、 ``Response`` で返す内容のタイプを決定するのに使われます。実際にリクエストフォーマットは、:method:`Symfony\\Component\\HttpFoundation\\Request::getRequestFormat` メソッドを介して ``Response`` オブジェクトの ``Content-Type`` ヘッダのマイムタイプをセットするのに使われます。内部的には、 Symfony は ``html`` や ``json`` などの一般的なフォーマットのマップを持っており、それぞれ対応するマイムタイプ ``text/html`` や ``application/json`` に結び付けられます。もちろん、フォーマットとマイムタイプのエントリも簡単に追加することができます。この記事では、 ``jsonp`` フォーマットとそれに対応するマイムタイプの追加方法を説明します。

``kernel.request`` リスナを作成する
-------------------------------------

新しいマイムタイプの定義するキーは、 Symfony カーネルによってディスパッチされる ``kernel.request`` イベントを "listen" するクラスを作成することです。 ``kernel.request`` イベントは、 Symfony のリクエスト処理の初期でディスパッチされるので、リクエストオブジェクトを変更することができるのです。

次のクラスを作成し、あなたのプロジェクトのバンドルのパスに入れ替えてください。
::

    // src/Acme/DemoBundle/RequestListener.php
    namespace Acme\DemoBundle;

    use Symfony\Component\HttpKernel\HttpKernelInterface;
    use Symfony\Component\HttpKernel\Event\GetResponseEvent;

    class RequestListener
    {
        public function onKernelRequest(GetResponseEvent $event)
        {
            $event->getRequest()->setFormat('jsonp', 'application/javascript');
        }
    }

リスナを登録する
-------------------------

他のリスナのように、 ``kernel.event_listener`` タグを追加して、コンフィギュレーションファイルにリスナとして追加、登録する必要があります。

.. configuration-block::

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <service id="acme.demobundle.listener.request" class="Acme\DemoBundle\RequestListener">
                <tag name="kernel.event_listener" event="kernel.request" method="onKernelRequest" />
            </service>

        </container>

    .. code-block:: yaml

        # app/config/config.yml
        services:
            acme.demobundle.listener.request:
                class: Acme\DemoBundle\RequestListener
                tags:
                    - { name: kernel.event_listener, event: kernel.request, method: onKernelRequest }

    .. code-block:: php

        # app/config/config.php
        $definition = new Definition('Acme\DemoBundle\RequestListener');
        $definition->addTag('kernel.event_listener', array('event' => 'kernel.request', 'method' => 'onKernelRequest'));
        $container->setDefinition('acme.demobundle.listener.request', $definition);

これで、 ``acme.demobundle.listener.request`` サービスが設定されているので、 Symfony のカーネルが ``kernel.request`` イベントをディスパッチする際に通知されます。

.. tip::

    コンフィギュレーション拡張クラスでリスナを定義することもできます。詳細は、 :ref:`service-container-extension-configuration` を参照してください。

.. 2011/11/18 ganchiku 54c8bfeda8879f9952bfed62050b2ac24d753b83

