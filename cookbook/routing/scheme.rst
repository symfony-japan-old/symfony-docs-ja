.. index::
   single: Routing; Scheme requirement

ルートで常に HTTPS を強制させる方法
===================================

サイトの作成時に、ルートのいくつかをセキュアにし、常に HTTPS プロトコルを介するようにしたいというときもあると思います。ルーティングコンポーネントは、 ``_scheme`` の必須条件を介して HTTPS スキームを強制させることができます。

.. configuration-block::

    .. code-block:: yaml

        secure:
            pattern:  /secure
            defaults: { _controller: AcmeDemoBundle:Main:secure }
            requirements:
                _scheme:  https

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="secure" pattern="/secure">
                <default key="_controller">AcmeDemoBundle:Main:secure</default>
                <requirement key="_scheme">https</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('secure', new Route('/secure', array(
            '_controller' => 'AcmeDemoBundle:Main:secure',
        ), array(
            '_scheme' => 'https',
        )));

        return $collection;

上記のコンフィギュレーションでは ``secure`` ルートに必ず HTTPS を使うようにしています。

``secure`` URLを生成する際に、現在の通信プロトコルが HTTP であれば Symfony は自動的に HTTPS プロトコルを使用した絶対 URL を生成します。

.. code-block:: text

    # If the current scheme is HTTPS
    {{ path('secure') }}
    # generates /secure

    # If the current scheme is HTTP
    {{ path('secure') }}
    # generates https://example.com/secure

この条件は、受け取ったリクエストにも強制化されます。もし HTTP で ``/secure`` パスにアクセスを試みると、 HTTPS プロトコルの同じ URL に自動的にリダイレクトされます。

上記の例では、 ``_scheme`` に ``https`` を指定していますが、 ``http`` と指定して、URLに HTTP プロトコルを強制させることもできます。

.. note::

    セキュリティコンポーネントは、``requires_channel`` 設定をすることでも HTTPS プロトコルを強制化することができます。この代替手段は ``/admin`` 以下の URL など、サイトの "一部分" をセキュアするのにより適しています。また、サードパーティのバンドルで定義された URL をセキュアにしたいときにも適しています。

.. 2011/10/24 ganchiku 39785a41b0fbf30c383d4ece7d14a8d7353f001

