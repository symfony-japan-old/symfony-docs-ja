.. index::
   single: Configuration; Semantic
   single: Bundle; Extension Configuration

セマンティックコンフィギュレーションを通してバンドルを設定する方法
==================================================================

アプリケーションのコンフィギュレーションファイル (通常は ``app/config/config.yml``)を見てみると、 ``framework`` 、 ``twig`` 、 ``doctrine`` といった、たくさんの異なるコンフィギュレーションの "namespaces" があります。これらの設定は、それぞれ指定のバンドルに適用されます。アプリケーション開発者には高レベルな設定を用意して、バンドルにはその内容に基づき低いレベルの複雑な変更を担うようにしています。

例えば、以下では ``FrameworkBundle`` に、多くのサービスを定義して、フォームの統合を可能にしています。これは他の関係するコンポーネントの統合でも同様です。:

.. configuration-block::

    .. code-block:: yaml
    
        framework:
            # ...
            form:            true

    .. code-block:: xml
    
        <framework:config>
            <framework:form />
        </framework:config>

    .. code-block:: php
    
        $container->loadFromExtension('framework', array(
            // ...
            'form'            => true,
            // ...
        ));

バンドルを作成する際に、コンフィギュレーションを取り扱うのに２つの方法があります。:

1. **普通のサービスコンフィギュレーション** (*簡単*):

    サービスを、あなたのバンドル内の ``services.yml`` などのコンフィギュレーションファイルで指定することができ、メインとなるアプリケーションのコンフィギュレーションからインポートできます。この方法はとても簡単でかつ素早く効果的です。 :ref:`parameters<book-service-container-parameters>` を使用すると、アプリケーションのコンフィギュレーションからあなたのバンドルをカスタマイズできるといった柔軟性も兼ね備えることになります。詳細は、 ":ref:`service-container-imports-directive`" を参照してください。

2. **セマンティックコンフィギュレーション** (*高度*):

    この方法は、上記で説明したコアバンドルで使用されているコンフィギュレーションの方法です。基本の考えは、ユーザに個々のパラメータをオーバーライドさせるのではなく、コンフィギュレーションを少なくし、限定したオプションのみをユーザに設定させることになります。バンドルの開発者として、あなたは "Extension" クラスの内部にあるコンフィギュレーションやロードサービスを通じてパースを行います。この方法では、メインのアプリケーションのコンフィギュレーションからは何もインポートする必要がありません。 "Extension" クラスが全ての処理を担います。

この記事で学ぶことになる２つ目のオプションの方がより柔軟ですが、その一方で準備に時間がかかってしまいます。どちらの方法を使用するべきか悩んだ際には、まず１つ目の方法で試してください。２つ目の方法は必要になった際に使用してください。

２つ目の方法の方には次のアドバンテージがあります。:

* 単にパラメータを定義するだけよりもはるかに強力です。パラメータに指定する際には、特定のオプションの値が、多くのサービス定義をしなければいけなくなるかもしれません。

* コンフィギュレーションの階層を可能にします。

* ``config_dev.yml`` と ``config.yml`` のようなコンフィギュレーションファイルのオーバライドをする際に、スマートなマージが可能です。

* :ref:`Configuration Class<cookbook-bundles-extension-config-class>` を使用すれば、コンフィギュレーションバリデーションが使用できます。

* あなたが XSD を作成しており、開発者が XML を使用する際に IDE の自動補完が可能になります。

.. sidebar:: バンドルのパラメータのオーバライド

    バンドルが Extension クラスを提供する際には、一般的なルールとして、そのバンドルからどのサービスコンテナパラメータもオーバーライドしてはいけません。なぜなら、 Extension クラスは、同クラスによって使用可能になるコンフィギュレーション内にも、必要な設定全てを指定しなければならないからです。つまり、 Extension クラスが、後方互換性を維持するには、パブリックにサポートされているコンフィギュレーションの設定を全て定義しないといけないのです。

.. index::
   single: Bundles; Extension
   single: Dependency Injection, Extension

Extension クラスの作成
----------------------

バンドルに、セマンティックコンフィギュレーションで指定することを決めたら、まず、処理を担当する "Extension" クラスを新しく作成する必要があります。このクラスは、作成するバンドルの ``DependencyInjection`` ディレクトリにください。また、クラス名は、バンドル名の ``Bundle`` 接尾辞を ``Extension`` で置き換えて作成してください。例えば、 ``AcmeHelloBundle`` バンドルの Extension クラスであれば、 ``AcmeHelloExtension`` になります。

::

    // Acme/HelloBundle/DependencyInjection/AcmeHelloExtension.php
    use Symfony\Component\HttpKernel\DependencyInjection\Extension;
    use Symfony\Component\DependencyInjection\ContainerBuilder;

    class AcmeHelloExtension extends Extension
    {
        public function load(array $configs, ContainerBuilder $container)
        {
            // where all of the heavy logic is done
        }

        public function getXsdValidationBasePath()
        {
            return __DIR__.'/../Resources/config/';
        }

        public function getNamespace()
        {
            return 'http://www.example.com/symfony/schema/';
        }
    }

.. note::

    ``getXsdValidationBasePath`` メソッドと ``getNamespace`` メソッドは、バンドルが コンフィギュレーションのためにオプションの XSD を提供する際のみ必要です。

上記のクラス名があれば、どのコンフィギュレーションファイルからも ``acme_hello`` コンフィギュレーションネームスペースを定義できるようになります。 ``acme_hello`` ネームスペースは、クラス名の ``Extension`` という語を除去し、残りを小文字化してアンダースコア化されたものになります。例えば、 ``AcmeHelloExtension`` の場合は、 ``acme_hello`` になります。

このネームスペースの下で早速コンフィギュレーションを指定することができます。:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        acme_hello: ~

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

           <acme_hello:config />
           ...

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('acme_hello', array());

.. tip::

    上記のようにネーミングの慣習に沿うことによって、カーネルにバンドルを登録すると常に Extension クラスの ``load()`` メソッドが呼ばれます。つまり、ユーザが ``acme_hello`` エントリ等を書かずに、コンフィギュレーションを提供しなくても、``load()`` メソッドが呼ばれ、空の ``$configs`` 配列を受け取ったことになります。必要であれば、バンドルの細かなデフォルトも指定することができます。

``$configs`` 配列のパージング
-----------------------------

ユーザがコンフィギュレーションファイルに ``acme_hello`` ネームスペースをインクルードすると、そのコンフィギュレーションがコンフィギュレーションの配列の後尾に加えられ、あなたの Extension の ``load()`` メソッドに渡されます。 Symfony2 は自動的に XML や YAML を配列に変換します。

次のコンフィギュレーションを見てみましょう。:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        acme_hello:
            foo: fooValue
            bar: barValue

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

            <acme_hello:config foo="fooValue">
                <acme_hello:bar>barValue</acme_hello:bar>
            </acme_hello:config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('acme_hello', array(
            'foo' => 'fooValue',
            'bar' => 'barValue',
        ));

``load()`` メソッドに渡される配列は以下のようになります。

::

    array(
        array(
            'foo' => 'fooValue',
            'bar' => 'barValue',
        )
    )

これは、ただのコンフィギュレーションの値の配列ではなく、 *多重配列* であることに注意してください。例えば、 ``config_dev.yml`` コンフィギューレーションファイルに ``acme_hello`` があると、直下に異なる値を追加します。そして、受け取る配列は以下のようになります。

::

    array(
        array(
            'foo' => 'fooValue',
            'bar' => 'barValue',
        ),
        array(
            'foo' => 'fooDevValue',
            'baz' => 'newConfigEntry',
        ),
    )
２つの配列の順番は最初に設定したもの順になります。

そして、これらのコンフィギューレーションをどうやってマージするのを決定するのかは、あなたが実装する内容です。例えば、２つ目の値が最初の値をオーバーライドしたり、マージしたりするなどできます。

後に、 :ref:`Configuration Class<cookbook-bundles-extension-config-class>` のセクションにあるように、その実装方法に関する強固なやり方を学びます。しかし、今回は、単純なマージをするだけにしましょう。

::

    public function load(array $configs, ContainerBuilder $container)
    {
        $config = array();
        foreach ($configs as $subConfig) {
            $config = array_merge($config, $subConfig);
        }

        // now use the flat $config array
    }

.. caution::

    上記のマージのテクニックが、開発しているバンドルのロジックに合致しているか確かめてください。今回は、単なる例ですので、盲目的に合うのは良くりません。

``load()`` メソッドの使用
-------------------------

``load()`` メソッドの中で、 ``$container`` 変数が、コンテナを指しており、このコンテナのみが、使用しているネームスペースコンフィギューレーションについての情報を保持しています。コンテナは、他のバンドルからロードされたサービスの情報は保持していません。 ``load()`` メソッドのゴールは、開発しているバンドルが必要なメソッドやサービスのコンフィギュレーションを追加して、コンテナを操ることです。

外部のコンフィギュレーションリソースをロードする
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

共通にタスクとして、外部のコンフィギュレーションファイルをロードすることが挙げられます。外部のコンフィギュレーションには、開発しているバンドルが必要としているサービスの大部分が含まれることがあります。例えば、開発しているバンドルのサービスコンフィギュレーションのほとんどを ``services.xml`` ファイルで定義していることを想定してください。

::

    use Symfony\Component\DependencyInjection\Loader\XmlFileLoader;
    use Symfony\Component\Config\FileLocator;

    public function load(array $configs, ContainerBuilder $container)
    {
        // prepare your $config variable

        $loader = new XmlFileLoader($container, new FileLocator(__DIR__.'/../Resources/config'));
        $loader->load('services.xml');
    }

コンフィギュレーションの値に基づいて、さらにこの条件を追加するかもしれません。例えば、 ``enabled`` オプションが渡されて、 ``true`` と設定されているときのみ、サービス一式をロードしたいとしましょう。

::

    public function load(array $configs, ContainerBuilder $container)
    {
        // prepare your $config variable

        $loader = new XmlFileLoader($container, new FileLocator(__DIR__.'/../Resources/config'));
        
        if (isset($config['enabled']) && $config['enabled']) {
            $loader->load('services.xml');
        }
    }

サービスのコンフィギュレーションとパラメータの設定
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

サービスのコンフィギュレーションをロードしてしまった後で、入力値に基づいてコンフィギュレーションを変更する必要があるかもしれません。例えば、内部で使う文字列の "type" を第一引数として渡すサービスがあるとしましょう。バンドルを使用するユーザが簡単に使うことができるようにするには、 ``services.xml`` のようなサービスコンフィギュレーションファイルの内で、このサービスを定義します。第一引数として ``acme_hello.my_service_type`` を空のパラメータを使用します。

.. code-block:: xml

    <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
    <container xmlns="http://symfony.com/schema/dic/services"
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
        xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

        <parameters>
            <parameter key="acme_hello.my_service_type" />
        </parameters>

        <services>
            <service id="acme_hello.my_service" class="Acme\HelloBundle\MyService">
                <argument>%acme_hello.my_service_type%</argument>
            </service>
        </services>
    </container>

しかし、なぜ空のパラメータを定義して、あなたのサービスに渡すようするのでしょうか？答えは、このパラメータは、受け取るコンフィギュレーションの値を元に、あなたの Extension クラスで設定するからです。例えば、 ``my_type`` のキーとして、ユーザにこの *種類(type)* オプションを定義可能にしているとしましょう。 ``load()`` メソッドの後にこれを追加したコードが以下になります。

::

    public function load(array $configs, ContainerBuilder $container)
    {
        // prepare your $config variable

        $loader = new XmlFileLoader($container, new FileLocator(__DIR__.'/../Resources/config'));
        $loader->load('services.xml');

        if (!isset($config['my_type'])) {
            throw new \InvalidArgumentException('The "my_type" option must be set');
        }

        $container->setParameter('acme_hello.my_service_type', $config['my_type']);
    }

これで、ユーザは ``my_type`` コンフィギュレーション値を特定することができるので、効果的に設定することができます。:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        acme_hello:
            my_type: foo
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" ?>

        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:acme_hello="http://www.example.com/symfony/schema/"
            xsi:schemaLocation="http://www.example.com/symfony/schema/ http://www.example.com/symfony/schema/hello-1.0.xsd">

            <acme_hello:config my_type="foo">
                <!-- ... -->
            </acme_hello:config>

        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('acme_hello', array(
            'my_type' => 'foo',
            // ...
        ));

グローバルパラメータ
~~~~~~~~~~~~~~~~~~~~

コンテナを設定する際に、次のグローバルパラメータが使用可能になっています。:

* ``kernel.name``
* ``kernel.environment``
* ``kernel.debug``
* ``kernel.root_dir``
* ``kernel.cache_dir``
* ``kernel.logs_dir``
* ``kernel.bundle_dirs``
* ``kernel.bundles``
* ``kernel.charset``

.. caution::

    ``_`` から始まる全てのパラメータとサービス名は、フレームワークの予約語ですので、バンドルで定義しないでください。

.. _cookbook-bundles-extension-config-class:

コンフィギュレーションクラスを使ったバリデーションとマージ
----------------------------------------------------------

これまでは、手動でコンフィギュレーションの配列のマージや、 PHP の ``isset()`` 関数を使用してコンフィグ値の存在のチェックをしてきました。オプションの *コンフィギュレーション* システムを使用すれば、マージ、バリデーション、デフォルト値、フォーマットの正規化をすることができます。

.. note::

    特定のフォーマット(ほとんどの場合 XML)は、最終的にほんの少し異なるコンフィギュレーション配列になり、これらの配列が他の全てと一致するために "正規化" される必要があることがあります。そのために、フォーマットの正規化が必要です。

このシステムのアドバンテージを受けるために、 ``Configuration`` クラスを作成し、そのクラス内でコンフィギュレーションを定義するツリーを構築しましょう。

::

    // src/Acme/HelloBundle/DependencyExtension/Configuration.php
    namespace Acme\HelloBundle\DependencyInjection;

    use Symfony\Component\Config\Definition\Builder\TreeBuilder;
    use Symfony\Component\Config\Definition\ConfigurationInterface;

    class Configuration implements ConfigurationInterface
    {
        public function getConfigTreeBuilder()
        {
            $treeBuilder = new TreeBuilder();
            $rootNode = $treeBuilder->root('acme_hello');

            $rootNode
                ->children()
                    ->scalarNode('my_type')->defaultValue('bar')->end()
                ->end()
            ;

            return $treeBuilder;
        }

上記の例は、 *とても* 簡単ですが、 ``load()`` メソッドを使用してこのくのクラスを使用することができ、あなたのコンフィギュレーションをマージでき、バリデーションを強制化させることができます。 ``my_type`` 以外のオプションが渡されると、サポートしていないオプションが渡ったことを説明する例外を返します。

::

    use Symfony\Component\Config\Definition\Processor;
    // ...

    public function load(array $configs, ContainerBuilder $container)
    {
        $processor = new Processor();
        $configuration = new Configuration();
        $config = $processor->processConfiguration($configuration, $configs);
    
        // ...
    }

``processConfiguration()`` メソッドは、 ``Configuration`` クラス内で定義されたコンフィギュレーションツリーを使い、全てのコンフィギュレーションの配列を合わせてバリデート、正規化、マージをします。

``Configuration`` クラスは、ここでの紹介よりももっと複雑になりえます。 ``Configuration`` クラスは、配列ノード、 "prototype" ノード、高度なバリデーション、 XMLに特化した正規化、高度なマージのサポートをしています。この実際に動いているコード見る最も良い方法は、コアのコンフィギュレーションクラスをしてくることです。 `FrameworkBundle Configuration`_ や `TwigBundle Configuration`_ がその一例です。

.. index::
   pair: Convention; Configuration

Extension の規格
----------------

Extension を作成する際に、次の簡単な規約に従ってください。:

* Extension は、 ``DependencyInjection`` に格納すること。

* Extension は、必ずバンドル名の後に ``Extension`` 接尾辞を付けること。 ``AcmeHelloBundle`` であれば、 ``AcmeHelloExtension`` になります。

* Extension は、 XSD スキーマを用意すること。

これらの簡単な規約に従えば、あなたの作るエスクテンションは、Symfony2 によって自動的に登録されます。もし、規約に従わないのであれば、あなたのバンドル内で :method:`Symfony\\Component\\HttpKernel\\Bundle\\Bundle::build` メソッドをオーバーライドしてください。

::

    use Acme\HelloBundle\DependencyInjection\UnconventionalExtensionClass;

    class AcmeHelloBundle extends Bundle
    {
        public function build(ContainerBuilder $container)
        {
            parent::build($container);

            // register extensions that do not follow the conventions manually
            $container->registerExtension(new UnconventionalExtensionClass());
        }
    }

上記のケースでは、 Extension クラスは、必ず ``getAlias()`` メソッドを実装して、バンドル名にちなんだユニークなエイリアスを返す必要があります(例えば、 ``acme_hello``)。標準的な ``Extension`` で終わるクラス名を付けるのを無視しているので、必須の作業となります。

さらに、あなたの Extension の ``load()`` メソッドは、コンフィギュレーションファイル内で、一度でも ``acme_hello`` エイリアスを特定化した *ときのみ* 呼ばれます。繰り返しますが、 Extension クラスが上記のように標準的な方法に従っていないので、自動的に登録されることはないのです。

.. _`FrameworkBundle Configuration`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/FrameworkBundle/DependencyInjection/Configuration.php
.. _`TwigBundle Configuration`: https://github.com/symfony/symfony/blob/master/src/Symfony/Bundle/TwigBundle/DependencyInjection/Configuration.php

.. 2011/12/27 ganchiku addd5fcbf41149901e8915e1215f1cdfd2d3582a

