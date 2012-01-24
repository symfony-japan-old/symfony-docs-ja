.. index::
   single: Routing; Allow / in route parameter

ルートパラメータに "/" 文字を使えるようにする方法
=================================================

スラッシュ ``/`` を含むパラメータで URL を組み立てる必要があるときもあるでしょう。典型的なルートの ``/hello/{name}`` を例にして見てみましょう。デフォルトでは、 ``/hello/Fabien`` はこのルートにマッチします。しかし、\ ``/hello/Fabien/Kris`` はマッチしません。これは Symfony がルート部分のセパレータとしてスラッシュを使用しているからです。

この記事では、\ ``/hello/Fabien/Kris`` が ``/hello/{name}`` ルートにマッチして ``{name}`` が ``Fabien/Kris`` となるようにするための方法を説明します。

ルートの設定
------------

デフォルトでは、 Symfony のルーティングコンポーネントは、正規表現パターンの ``[^/]+`` にマッチするパラメータを必要とします。この正規表現でわかるように、 ``/`` 以外の全ての文字が使用可能になっています。

ここでスラッシュ ``/`` をパラメータで使用可能にするには、次のように正規表現パターンを修正してより寛容な指定にする必要があります。

.. configuration-block::

    .. code-block:: yaml

        _hello:
            pattern: /hello/{name}
            defaults: { _controller: AcmeDemoBundle:Demo:hello }
            requirements:
                name: ".+"

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="_hello" pattern="/hello/{name}">
                <default key="_controller">AcmeDemoBundle:Demo:hello</default>
                <requirement key="name">.+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('_hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeDemoBundle:Demo:hello',
        ), array(
            'name' => '.+',
        )));

        return $collection;

    .. code-block:: php-annotations

        use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;

        class DemoController
        {
            /**
             * @Route("/hello/{name}", name="_hello", requirements={"name" = ".+"})
             */
            public function helloAction($name)
            {
                // ...
            }
        }

これで ``{name}`` パラメータは ``/`` 文字を含むことができるようになりました。

.. 2011/10/24 ganchiku d4cee249baa41c03d940b8bf16511de686dea920

