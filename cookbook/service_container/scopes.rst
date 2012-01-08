スコープの使用方法
==================

このエントリは全て :doc:`/book/service_container` に関連する高度なトピックであるスコープに関してになります。このエントリは次の２つのケースで役に立ちます。まず、サービスの作成時に "scopes" を言及しているエラーに遭遇したときです。次に `request` サービスに依存するサービスを作成する必要があったときです。

スコープを理解する
------------------

サービスのスコープは、サービスのインスタンスがコンテナによって使われる期間を制御します。 DI コンポーネントは、２つの一般的なスコープを提供します。
:

- `container` (デフォルトはこちら): このコンテナからサービスを呼び出す際は、毎回同じインスタンスが使用されます。

- `prototype`: サービスを呼び出す度に、新しいインスタンスが作成されます。

FrameworkBundle も３つ目のスコープである `request` を定義しています。 `request` スコープは、リクエストに関係しており、サブリクエスト毎に新しいインスタンスが作成されます。また、このスコープは CLI などリクエスト以外からの利用は不可能になっています。

スコープは、サービスの依存に制約を追加します。サービスはより狭いスコープのサービスに依存することはできません。例えば、一般的な `my_foo` サービスを作成して、 `request` コンポーネントに注入(inject)を試みると、コンテナのコンパイル時に :class:`Symfony\\Component\\DependencyInjection\\Exception\\ScopeWideningInjectionException` 例外が投げられます。詳細は、下のサイドバーの情報を読んでください。

.. sidebar:: Scopes and Dependencies

    `my_mailer` サービスを設定していることを想像してください。サービスのスコープはまだ設定していないので、デフォルトの `container` になります。つまり、 `my_mailer` サービスについてコンテナに問いかける際は常に同じオブジェクトを受け取ることになります。ほとんどの場合は、これが必要な処理なので問題はありません。

    しかし、 `my_mailer` サービスで `request` サービスが必要となると想像してください。例えば、現在のリクエストから URL を読んでいるときなどの場合です。そのために、コンストラクタの引数を追加します。なぜ、これが問題となるのか見てきましょう。

    * `my_mailer` を問いかけると、 `my_mailer` のインスタンス( *MailerA* と命名します)が作成され、 `request` サービスとしてリクエストへ渡ります( *RequestA* と命名します)。ここまでは、問題ありません。

    * そして Symfony でサブリクエストを作成して、他のコントローラを実行します。例えば、 Twig の関数 {% render ...%}` のようにです。内部的には、最初の `request` サービス(*RequestA*) は、新しいリクエストのインスタンス(*RequestB*) に置き換わります。これは、バックグラウンドの動作であり、全く問題がありません。


    * 二度目に呼ばれるコントローラで、もう一度 `my_mailer` サービスを問いかけることになります。このサービスは、 `container` スコープにあるので、同じインスタンス (*MailerA*) が再利用されます。ここで問題が出てきてしまいます。 *MailerA* インスタンスは、置き換わる前の最初の *RequestA* のオブジェクトを含んでいるので、現在の正しいリクエストオブジェクトにはなっていません(今となっては *RequestB* が正しい `request` サービスとなっています)。些細なことですが、このミスマッチが大きな問題になるときもあります。これが例外が投げられる理由となります。

      これが、スコープの *存在理由* で、問題の原因となります。次に、一般的な解決方法を見ていきましょう。

.. note::

    もちろんサービスは、より大きなスコープのサービスには問題無く依存することができます。

コンフィギュレーションでスコープを設定する
------------------------------------------

サービスのスコープは、サービスのコンフィギュレーションで定義することができます。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        services:
            greeting_card_manager:
                class: Acme\HelloBundle\Mail\GreetingCardManager
                scope: request

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <services>
            <service id="greeting_card_manager" class="Acme\HelloBundle\Mail\GreetingCardManager" scope="request" />
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition(
            'greeting_card_manager',
            new Definition('Acme\HelloBundle\Mail\GreetingCardManager')
        )->setScope('request');

スコープを指定しなければ、デフォルトの `container` が使われますし、ほとんどの場合これで問題がありません。サービスがより狭いスコープの他のサービス(一般的には `request` サービス)に依存しているときのみ、スコープを設定する必要が出てきます。

狭いスコープのサービスを使用する
--------------------------------

他のサービスに依存したサービスを作成する際に、最も良い方法は、同じスコープに入れるか、より狭いスコープに入れてください。ほとんどの場合、 `request` サービス内に新しいサービスを入れることになります。

しかし、これが毎回可能であるとは限りません。例えば、 Twig エクステンションでは、Twig 環境の依存のため、 `container` スコープ内にいる必要があります。こういったケースでは、コンテナ全体をサービスに渡し、正しいインスタンスを持っているか確認するため、毎回コンテナから依存を参照する必要があります。
::

    namespace Acme\HelloBundle\Mail;

    use Symfony\Component\DependencyInjection\ContainerInterface;

    class Mailer
    {
        protected $container;

        public function __construct(ContainerInterface $container)
        {
            $this->container = $container;
        }

        public function sendEmail()
        {
            $request = $this->container->get('request');
            // Do something using the request here
        }
    }

.. warning::

    最初のセクションで説明した同じ問題を避けるために、サービスの将来の呼び出しに備えて、オブジェクトのプロパティにリクエストを保管しないでください(symfony が間違いを検出できないとき以外は)。

このクラスのサービスコンフィギュレーションは次のようになります。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            my_mailer.class: Acme\HelloBundle\Mail\Mailer
        services:
            my_mailer:
                class:     %my_mailer.class%
                arguments:
                    - "@service_container"
                # scope: container can be omitted as it is the default

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="my_mailer.class">Acme\HelloBundle\Mail\Mailer</parameter>
        </parameters>

        <services>
            <service id="my_mailer" class="%my_mailer.class%">
                 <argument type="service" id="service_container" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mail\Mailer');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array(new Reference('service_container'))
        ));

.. note::

    全てのコンテナをサービスに注入(inject)するのは、良い方法ではありません。必要なものだけにしてください。しかし、ごく稀に ``request`` スコープ内でサービスが必要な ``container`` スコープがあり、そしてさらにその ``container`` スコープ内にサービスがあるときに必要になります。

コントローラをサービスとして定義すれば、アクションメソッドの引数を渡してコンテナに注入しなくても ``Request`` オブジェクトを取得できます。詳細は、 :ref:`book-controller-request-argument` を参照してください。

.. 2011/11/08 ganchiku 063b1f6bb4b10fbafff24c846b0796a470978688

