.. note::

    * 対象バージョン : 2.3+
    * 翻訳更新日 : 2013/11/23

Dependency Injectionタグ
=============================

Dependency Injectionタグはサービスに、特別な方法でフラグを立てるのに使う短い文字列です。たとえば、Symfonyのコアのイベントに対するリスナーを登録したいときは ``kernel.event_listener`` タグを使います。

タグについてより詳しく知りたいときはサービスコンテナの章の ":ref:`book-service-container-tags`" セクションを読んで下さい。

以下はSymfony2の内部で利用可能なタグの一覧です。他のバンドルで使うタグについてはここには載っていません。

+-----------------------------------+---------------------------------------------------------------------------+
| タグ名                            | 使い方                                                                    |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.asset`_                  | asseticの現在のアセットマネージャーにアセットを追加します                 |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.factory_worker`_         | asseticのfactory workerを追加します                                       |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.filter`_                 | asseticのfilterを追加します                                               |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.formula_loader`_         | asseticの現在のアセットマネージャーにformula loaderを追加します           |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.formula_resource`_       | asseticの現在のアセットマネージャーにリソースを追加します                 |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.templating.php`_         | phpテンプレートエンジンが使われていない場合にサービスを削除します         |
+-----------------------------------+---------------------------------------------------------------------------+
| `assetic.templating.twig`_        | twigテンプレートエンジンが使われていないときにサービスを削除します        |
+-----------------------------------+---------------------------------------------------------------------------+
| `console.command`_                | コンソールコマンドを追加します                                            |
+-----------------------------------+---------------------------------------------------------------------------+
| `data_collector`_                 | プロファイラ用にカスタムデータコレクタを追加します                        |
+-----------------------------------+---------------------------------------------------------------------------+
| `doctrine.event_listener`_        | Doctrineのイベントリスナーを追加します                                    |
+-----------------------------------+---------------------------------------------------------------------------+
| `doctrine.event_subscriber`_      | Doctrineのイベントサブスクライバーを追加します                            |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type`_                      | カスタムフォームフィールドタイプを追加します                              |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type_extension`_            | カスタムフォームエクステンションを追加します                              |
+-----------------------------------+---------------------------------------------------------------------------+
| `form.type_guesser`_              | 独自のフォームタイプ推測器（Guessor）を追加します                         |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.cache_clearer`_           | キャッシュクリア時に呼び出すサービスを追加します                          |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.cache_warmer`_            | キャッシュウォームアップ時に呼び出すサービスを追加します                  |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.event_listener`_          | Symfonyのさまざまなイベント・フックにイベントリスナーを追加します         |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.event_subscriber`_        | Symfonyのさまざまなイベント・フックにサブスクライバーを追加します         |
+-----------------------------------+---------------------------------------------------------------------------+
| `kernel.fragment_renderer`_       | 新しいHTTPレンダリング方法を追加します                                    |
+-----------------------------------+---------------------------------------------------------------------------+
| `monolog.logger`_                 | カスタムログチャンネルを追加します　　　                                  |
+-----------------------------------+---------------------------------------------------------------------------+
| `monolog.processor`_              | カスタムログプロセッサーを追加します                                      |
+-----------------------------------+---------------------------------------------------------------------------+
| `routing.loader`_                 | ルーティング設定を読み込む独自のサービスを追加します                      |
+-----------------------------------+---------------------------------------------------------------------------+
| `security.voter`_                 | Symfonyの認証機構に独自のvoterを追加します                                |
+-----------------------------------+---------------------------------------------------------------------------+
| `security.remember_me_aware`_     | 次回から自動ログイン機能を使う                                            |
+-----------------------------------+---------------------------------------------------------------------------+
| `serializer.encoder`_             | ``serializer`` サービスに独自のエンコーダーを追加します                   |
+-----------------------------------+---------------------------------------------------------------------------+
| `serializer.normalizer`_          | ``serializer`` サービスに独自のノーマライザーを追加します                 |
+-----------------------------------+---------------------------------------------------------------------------+
| `swiftmailer.plugin`_             | SwiftMailerのプラグインを追加します　                                     |
+-----------------------------------+---------------------------------------------------------------------------+
| `templating.helper`_              | サービスをPHPテンプレートエンジンでヘルパーとして利用できるようにする     |
+-----------------------------------+---------------------------------------------------------------------------+
| `translation.loader`_             | 独自の翻訳ローダー追加します                                              |
+-----------------------------------+---------------------------------------------------------------------------+
| `translation.extractor`_          | テンプレートから翻訳が必要なメッセージを展開する独自サービスを追加します  |
+-----------------------------------+---------------------------------------------------------------------------+
| `translation.dumper`_             | 翻訳メッセージをダンプする独自サービスを追加します                        |
+-----------------------------------+---------------------------------------------------------------------------+
| `twig.extension`_                 | 独自のTwigエクステンションを追加します                                    |
+-----------------------------------+---------------------------------------------------------------------------+
| `twig.loader`_                    | Twigテンプレートを読み込む独自サービスを追加します                        |
+-----------------------------------+---------------------------------------------------------------------------+
| `validator.constraint_validator`_ | 独自のバリデーション制約を追加します                                      |
+-----------------------------------+---------------------------------------------------------------------------+
| `validator.initializer`_          | バリデーション前にオブジェクトを初期化する独自サービスを追加します        |
+-----------------------------------+---------------------------------------------------------------------------+

assetic.asset
-------------

**目的**: 現在のアセットマネージャにアセットを追加します

assetic.factory_worker
----------------------

**目的**: Factory workerを追加する

Factory worker は ``Assetic\Factory\Worker\WorkerInterface`` を実装したクラスです。
``process($asset)`` メソッドは、各アセットの作成後に呼び出され、アセットを変更したり、別の新しいアセットを返すことができます。

新しいFactory workerを追加するには、まずクラスを作ります。::

    use Assetic\Asset\AssetInterface;
    use Assetic\Factory\Worker\WorkerInterface;

    class MyWorker implements WorkerInterface
    {
        public function process(AssetInterface $asset)
        {
            // ... $asset を変更するか新しいものを返します
        }

    }

そして、タグ付けされたサービスとして登録します。

.. configuration-block::

    .. code-block:: yaml

        services:
            acme.my_worker:
                class: MyWorker
                tags:
                    - { name: assetic.factory_worker }

    .. code-block:: xml

        <service id="acme.my_worker" class="MyWorker>
            <tag name="assetic.factory_worker" />
        </service>

    .. code-block:: php

        $container
            ->register('acme.my_worker', 'MyWorker')
            ->addTag('assetic.factory_worker')
        ;

assetic.filter
--------------

**目的**: フィルタを追加します

AsseticBundle はこのタグを使って共通のフィルタを登録します。このタグを使うと、独自のフィルタを登録することができます。

まず、フィルタクラスを作ります。::

    use Assetic\Asset\AssetInterface;
    use Assetic\Filter\FilterInterface;

    class MyFilter implements FilterInterface
    {
        public function filterLoad(AssetInterface $asset)
        {
            $asset->setContent('alert("yo");' . $asset->getContent());
        }

        public function filterDump(AssetInterface $asset)
        {
            // ...
        }
    }

次に、サービスを定義します。

.. configuration-block::

    .. code-block:: yaml

        services:
            acme.my_filter:
                class: MyFilter
                tags:
                    - { name: assetic.filter, alias: my_filter }

    .. code-block:: xml

        <service id="acme.my_filter" class="MyFilter">
            <tag name="assetic.filter" alias="my_filter" />
        </service>

    .. code-block:: php

        $container
            ->register('acme.my_filter', 'MyFilter')
            ->addTag('assetic.filter', array('alias' => 'my_filter'))
        ;

最後に、フィルタを適用します。

.. code-block:: jinja

    {% javascripts
        '@AcmeBaseBundle/Resources/public/js/global.js'
        filter='my_filter'
    %}
        <script src="{{ asset_url }}"></script>
    {% endjavascripts %}

:doc:`/cookbook/assetic/apply_to_option` で説明されているように、 ``assetic.filters.my_filter.apply_to`` を使ってフィルタを適用することもできます。
この場合、独自のフィルタサービスを独立した xml の設定ファイルで定義し、そのファイルのパスを ``assetic.filters.my_filter.resource`` という設定名で指し示す必要があります。

assetic.formula_loader
----------------------

**目的**: 現在のアセットマネージャーにformula loaderを追加します

Formula loader は ``Assetic\\Factory\Loader\\FormulaLoaderInterface`` を実装したクラスです。このクラスは特定の種類のリソース（たとえば、twigテンプレート）からアセットを読み込むことを担当します。Assetic には php と twig テンプレート用のローダーを提供してます。

``alias`` 属性を使ってローダーの名前を定義することができます。

assetic.formula_resource
------------------------

**目的**: 現在のアセットマネージャーにリソースを追加します

リソースは読み込むことのできる形式です。たとえば、twigテンプレートはリソースの一種です。

assetic.templating.php
----------------------

**目的**: phpテンプレートエンジンが使われていない場合にサービスを削除します

このタグがついているサービスは、 ``framework.templating.engines`` 設定セクションが php を含んでいない場合にコンテナから削除されます。

assetic.templating.twig
-----------------------

**目的**: twigテンプレートエンジンが使われていない場合にサービスを削除します

このタグがついているサービスは、 ``framework.templating.engines`` 設定セクションが twig を含んでいない場合にコンテナから削除されます。

console.command
---------------

.. versionadded:: 2.4
   サービスコンテナによるコマンド登録は 2.4 から追加されました。

**目的**: アプリケーションにコマンドを追加します

サービスコンテナによる独自コマンド登録について、詳細は :ref:`cookbook-console-dic` を読んでください。

data_collector
--------------

**目的**: プロファイラ用にカスタムデータコレクタを追加する

カスタムデータコレクタ作成について詳細は :doc:`/cookbook/profiler/data_collector` を参照してください。

doctrine.event_listener
-----------------------

**目的**: Doctrineのイベントリスナーを追加する

Doctrineのイベントリスナー作成について詳細は :doc:`/cookbook/doctrine/event_listeners_subscribers` を参照してください。

doctrine.event_subscriber
-------------------------

**目的**: Doctrineのイベントサブスクライバ―を追加する

Doctrineのイベントサブスクライバ―作成について詳細は :doc:`/cookbook/doctrine/event_listeners_subscribers` を参照してください。

.. _dic-tags-form-type:

form.type
---------

**目的**: カスタムフォームフィールドタイプを追加します

カスタムフォームフィールドタイプの作成について詳細は :doc:`/cookbook/form/create_custom_field_type` を参照してください。

form.type_extension
-------------------

**目的**: カスタムフォームエクステンションを追加します

フォームタイプエクステンションを使うと、フォームのすべての項目の作成時にホックすることができます。たとえば、 CSRF トークンの追加は、あるフォームタイプエクステンションを使って行われています。 (:class:`Symfony\\Component\\Form\\Extension\\Csrf\\Type\\FormTypeCsrfExtension`).

フォームタイプエクステンションは、フォームの中のどの項目のどの部分でも動作を変更することができます。フォームタイプエクステンションを作るには、まず :class:`Symfony\\Component\\Form\\FormTypeExtensionInterface` を実装したクラスを作ります。
より簡単に、インターフェースではなく :class:`Symfony\\Component\\Form\\AbstractTypeExtension` を継承することもできます。::

    // src/Acme/MainBundle/Form/Type/MyFormTypeExtension.php
    namespace Acme\MainBundle\Form\Type;

    use Symfony\Component\Form\AbstractTypeExtension;

    class MyFormTypeExtension extends AbstractTypeExtension
    {
        // ... ここに動作を変更したいメソッドを書きます
        // buildForm(), buildView(), finishView(), setDefaultOptions() など
    }

Symfony に新しく追加したフォームエクステンションを使うことを知らせるために ``form.type_extension`` タグをつけます。

.. configuration-block::

    .. code-block:: yaml

        services:
            main.form.type.my_form_type_extension:
                class: Acme\MainBundle\Form\Type\MyFormTypeExtension
                tags:
                    - { name: form.type_extension, alias: field }

    .. code-block:: xml

        <service id="main.form.type.my_form_type_extension" class="Acme\MainBundle\Form\Type\MyFormTypeExtension">
            <tag name="form.type_extension" alias="field" />
        </service>

    .. code-block:: php

        $container
            ->register('main.form.type.my_form_type_extension', 'Acme\MainBundle\Form\Type\MyFormTypeExtension')
            ->addTag('form.type_extension', array('alias' => 'field'))
        ;

タグの ``alias`` はこのフォームエクステンションを適用する対象を表しています。たとえば、すべてのフォームのすべての項目に適用したい場合は "form" を使ってください。

form.type_guesser
-----------------

**目的**: 独自のフォームタイプ推測器を追加する

このタグを使うと :ref:`Form Guessing <book-forms-field-guessing>` に独自のロジックを追加することができます。
デフォルトでは、フォームタイプの推測はバリデーションメタデータとDoctrineのメタデータ（Doctrineを使用している場合）に基づいて行われます。

独自のフォームタイプ推測器を追加するには、まず、 :class:`Symfony\\Component\\Form\\FormTypeGuesserInterface` を実装したクラスを作成します。
次に、そのサービスに ``form.type_guesser`` タグ（オプションはありません）をつけます。

フォームタイプ推測器の書き方の例を知りたければ、 ``Form`` コンポーネントの ``ValidatorTypeGuesser`` を見てください。

kernel.cache_clearer
--------------------

**目的**: キャッシュクリア時に呼び出すサービスを追加する

キャッシュクリアは ``cache:clear`` コマンドを実行したときに行われます。バンドルでファイルをキャッシュするのであれば、キャッシュクリア時に動作する独自のキャッシュクリアサービスを追加すべきです。

独自のキャッシュクリアサービスを登録するには、まずサービスクラスを作ります。::

    // src/Acme/MainBundle/Cache/MyClearer.php
    namespace Acme\MainBundle\Cache;

    use Symfony\Component\HttpKernel\CacheClearer\CacheClearerInterface;

    class MyClearer implements CacheClearerInterface
    {
        public function clear($cacheDir)
        {
            // キャッシュクリアの動作を書きます
        }

    }

そして、コンテナに登録し、 ``kernel.cache_clearer`` タグをつけます:

.. configuration-block::

    .. code-block:: yaml

        services:
            my_cache_clearer:
                class: Acme\MainBundle\Cache\MyClearer
                tags:
                    - { name: kernel.cache_clearer }

    .. code-block:: xml

        <service id="my_cache_clearer" class="Acme\MainBundle\Cache\MyClearer">
            <tag name="kernel.cache_clearer" />
        </service>

    .. code-block:: php

        $container
            ->register('my_cache_clearer', 'Acme\MainBundle\Cache\MyClearer')
            ->addTag('kernel.cache_clearer')
        ;

kernel.cache_warmer
-------------------

**目的**: キャッシュウォームアップ時に呼び出すサービスを追加する

キャッシュウォームアップは ``cache:warmup`` コマンドまたは ``cache:clear`` コマンドを実行すると行われます（ ``cache:clear`` に ``--no-warmup`` オプションを付けなかったときに限る）。
アプリケーションの動作に必要なキャッシュを初期化し、最初にアクセスしたユーザーがキャッシュを自動的に生成する際の重い動作に悩まされるのを防止するのが目的です。

独自のキャッシュウォーマーを登録するには、まず、 the :class:`Symfony\\Component\\HttpKernel\\CacheWarmer\\CacheWarmerInterface` を実装するサービスを作成します。::

    // src/Acme/MainBundle/Cache/MyCustomWarmer.php
    namespace Acme\MainBundle\Cache;

    use Symfony\Component\HttpKernel\CacheWarmer\CacheWarmerInterface;

    class MyCustomWarmer implements CacheWarmerInterface
    {
        public function warmUp($cacheDir)
        {
            // キャッシュをウォームアップの動作を書きます
        }

        public function isOptional()
        {
            return true;
        }
    }

``isOptional`` メソッドは、そのキャッシュウォーマーを使用せずにアプリケーションを動作させることができるなら、trueを返すようにしてください。
Symfony 2.0 では、オプションのウォーマーは isOptional() の返り値に関わらず常に実行されるため、このメソッドは実際の効果がありません。

キャッシュウォーマーをSymfonyに登録するには、 ``kernel.cache_warmer`` タグを使ってください。
:

.. configuration-block::

    .. code-block:: yaml

        services:
            main.warmer.my_custom_warmer:
                class: Acme\MainBundle\Cache\MyCustomWarmer
                tags:
                    - { name: kernel.cache_warmer, priority: 0 }

    .. code-block:: xml

        <service id="main.warmer.my_custom_warmer" class="Acme\MainBundle\Cache\MyCustomWarmer">
            <tag name="kernel.cache_warmer" priority="0" />
        </service>

    .. code-block:: php

        $container
            ->register('main.warmer.my_custom_warmer', 'Acme\MainBundle\Cache\MyCustomWarmer')
            ->addTag('kernel.cache_warmer', array('priority' => 0))
        ;

``priority`` は必須ではなく、デフォルトの値は0です。
-255 から 255 までの値を設定することができ、キャッシュウォーマーはこの優先度の順に実行されます。

.. _dic-tags-kernel-event-listener:

kernel.event_listener
---------------------

**目的**: Symfonyのさまざまなイベント・フックにイベントリスナーを追加する

このタグを使うと、独自のクラスを Symfony のプロセスにフックさせることができます。

イベントリスナーの例については、 :doc:`/cookbook/service_container/event_listener` を参照してください。

実際のイベントリスナーの例は :doc:`/cookbook/request/mime_type` にもあります。

コアEvent Listenerリファレンス
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

リスナーを追加する場合、Symfony コアのリスナーについての知識が役立つでしょう。

.. note::

    下記の一覧に掲載されているリスナーは、アプリケーションの環境・設定・バンドルにより動作していないかもしれません。
    また、サードパーティのバンドルでは、下記に載っていないリスナーが動作しているかもしれません。

kernel.request
..............

+-------------------------------------------------------------------------------------------+-----------+
| リスナーのクラス名                                                                        | 優先度    |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`                  | 1024      |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\TestSessionListener`             | 192       |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\SessionListener`                 | 128       |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\RouterListener`                    | 32        |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\LocaleListener`                    | 16        |
+-------------------------------------------------------------------------------------------+-----------+
| :class:`Symfony\\Component\\Security\\Http\\Firewall`                                     | 8         |
+-------------------------------------------------------------------------------------------+-----------+

kernel.controller
.................

+-------------------------------------------------------------------------------------------+----------+
| リスナーのクラス名                                                                        | 優先度   |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\DataCollector\\RequestDataCollector`            | 0        |
+-------------------------------------------------------------------------------------------+----------+

kernel.response
...............

+-------------------------------------------------------------------------------------------+----------+
| リスナーのクラス名                                                                        | 優先度   |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\EsiListener`                       | 0        |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ResponseListener`                  | 0        |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\SecurityBundle\\EventListener\\ResponseListener`                 | 0        |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`                  | -100     |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\TestSessionListener`             | -128     |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`       | -128     |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\StreamedResponseListener`          | -1024    |
+-------------------------------------------------------------------------------------------+----------+

kernel.exception
................

+-------------------------------------------------------------------------------------------+----------+
| リスナーのクラス名                                                                        | 優先度   |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`                  | 0        |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener`                 | -128     |
+-------------------------------------------------------------------------------------------+----------+

kernel.terminate
................

+-------------------------------------------------------------------------------------------+----------+
| リスナーのクラス名                                                                        | 優先度   |
+-------------------------------------------------------------------------------------------+----------+
| :class:`Symfony\\Bundle\\SwiftmailerBundle\\EventListener\\EmailSenderListener`           | 0        |
+-------------------------------------------------------------------------------------------+----------+

.. _dic-tags-kernel-event-subscriber:

kernel.event_subscriber
-----------------------

**目的**: Symfonyのさまざまなイベント・フックにサブスクライバーを追加する

独自のサブスクライバーを利用するには、 ``kernel.event_subscriber`` タグをつけてサービスを登録してください。

.. configuration-block::

    .. code-block:: yaml

        services:
            kernel.subscriber.your_subscriber_name:
                class: Fully\Qualified\Subscriber\Class\Name
                tags:
                    - { name: kernel.event_subscriber }

    .. code-block:: xml

        <service id="kernel.subscriber.your_subscriber_name" class="Fully\Qualified\Subscriber\Class\Name">
            <tag name="kernel.event_subscriber" />
        </service>

    .. code-block:: php

        $container
            ->register('kernel.subscriber.your_subscriber_name', 'Fully\Qualified\Subscriber\Class\Name')
            ->addTag('kernel.event_subscriber')
        ;

.. note::

    イベントサブスクライバとして登録するサービスは :class:`Symfony\\Component\\EventDispatcher\\EventSubscriberInterface` を実装しなければなりません。

.. note::

    サービスがファクトリによって作成される場合は ``class`` パラメータを **絶対に** 正しく設定する必要があります。

kernel.fragment_renderer
------------------------

**目的**: 新しいHTMLレンダリング方法を追加します

``EsiFragmentRenderer``  のようなコアのレンダリング方法に加えて、新しいレンダリング方法を追加するには、 :class:`Symfony\\Component\\HttpKernel\\Fragment\\FragmentRendererInterface` を実装したクラスを作成して、サービスとして登録し、 ``kernel.fragment_renderer`` タグをつけます。

.. _dic_tags-monolog:

monolog.logger
--------------

**目的**: Monologのカスタムログチャンネルを追加します

Monolog を使うと、複数のログチャンネルでハンドラを共有することができます。
ロガーサービスは通常 ``app`` チャンネルを使いますが、このタグを使うと、ロガーをサービスに注入するときにチャンネルを変更することができます。

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Fully\Qualified\Loader\Class\Name
                arguments: ["@logger"]
                tags:
                    - { name: monolog.logger, channel: acme }

    .. code-block:: xml

        <service id="my_service" class="Fully\Qualified\Loader\Class\Name">
            <argument type="service" id="logger" />
            <tag name="monolog.logger" channel="acme" />
        </service>

    .. code-block:: php

        $definition = new Definition('Fully\Qualified\Loader\Class\Name', array(new Reference('logger'));
        $definition->addTag('monolog.logger', array('channel' => 'acme'));
        $container->register('my_service', $definition);

.. _dic_tags-monolog-processor:

monolog.processor
-----------------

**目的**: カスタムログプロセッサーを追加します

Monolog では、ログのレコードに追加のデータを含めるために、ロガーやハンドラにプロセッサーを追加できるようになっています。
プロセッサーはログのレコードを受け取り、 ``extra`` 属性に追加のデータを足した後にそのレコードを返さなければなりません。

ビルトインの ``IntrospectionProcessor`` を使って、ロガーを呼び出したファイル名・行数・クラス・メソッドをどのように追加するか、見てみましょう。

プロセッサーはグローバルに追加することができます。

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor');
        $container->register('my_service', $definition);

.. tip::

    サービスが ``__invoke`` を使って実行できない場合は、タグに ``method`` 属性を追加することで呼び出すメソッドを指定することができます。


``handler`` 属性を使って、プロセッサーを特定のhandlerに追加することもできます。

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, handler: firephp }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" handler="firephp" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('handler' => 'firephp');
        $container->register('my_service', $definition);

``channel`` 属性を使えば、プロセッサーを指定したログチャンネルだけに追加することもできます。
下記ではプロセッサーを Security コンポーネントで使用される ``security`` ログチャンネルだけに追加しています。

.. configuration-block::

    .. code-block:: yaml

        services:
            my_service:
                class: Monolog\Processor\IntrospectionProcessor
                tags:
                    - { name: monolog.processor, channel: security }

    .. code-block:: xml

        <service id="my_service" class="Monolog\Processor\IntrospectionProcessor">
            <tag name="monolog.processor" channel="security" />
        </service>

    .. code-block:: php

        $definition = new Definition('Monolog\Processor\IntrospectionProcessor');
        $definition->addTag('monolog.processor', array('channel' => 'security');
        $container->register('my_service', $definition);

.. note::

    ``handler`` と ``channel`` 属性は同時に使用することができません。
    ハンドラはすべてのチャンネルで共有されるためです。

routing.loader
--------------

**目的**: ルーティング設定を読み込む独自のサービスを追加します

独自のルーティングローダーを使うには、サービスとして登録し、 ``routing.loader`` タグをつけてください。

.. configuration-block::

    .. code-block:: yaml

        services:
            routing.loader.your_loader_name:
                class: Fully\Qualified\Loader\Class\Name
                tags:
                    - { name: routing.loader }

    .. code-block:: xml

        <service id="routing.loader.your_loader_name" class="Fully\Qualified\Loader\Class\Name">
            <tag name="routing.loader" />
        </service>

    .. code-block:: php

        $container
            ->register('routing.loader.your_loader_name', 'Fully\Qualified\Loader\Class\Name')
            ->addTag('routing.loader')
        ;

詳細は :doc:`/cookbook/routing/custom_route_loader` を参照してください。

security.remember_me_aware
--------------------------

**目的**: 次回から自動ログイン機能を使う

このタグは、次回から自動ログイン機能を動作させるために内部的に使われています。
アプリケーションに次回から自動ログイン機能を使う独自の認証方法があるなら、このタグを使う必要があるでしょう。

カスタムの認証ファクトリが :class:`Symfony\\Bundle\\SecurityBundle\\DependencyInjection\\Security\\Factory\\AbstractFactory` を継承していて、認証リスナーが :class:`Symfony\\Component\\Security\\Http\\Firewall\\AbstractAuthenticationListener` を継承しているなら、その認証には既にこのタグが付けられており、自動的に動作するでしょう。

security.voter
--------------

**目的**: Symfonyの認証機構に独自のvoterを追加します

Symfony のセキュリティコンテクストで ``isGranted`` を呼び出したとき、ユーザーのアクセス権限を調べるために内部的に voter の仕組みが動作しています。
``security.voter`` タグを使うと、アプリケーションに独自の voter を追加することができます。

詳細は :doc:`/cookbook/security/voters` を参照してください。

.. _reference-dic-tags-serializer-encoder:

serializer.encoder
------------------

**目的**: ``serializer`` サービスに独自のエンコーダーを追加します

このタグをつけるクラスは :class:`Symfony\\Component\\Serializer\\Encoder\\EncoderInterface` と :class:`Symfony\\Component\\Serializer\\Encoder\\DecoderInterface` を実装している必要があります。

詳細は :doc:`/cookbook/serializer` を参照してください。

.. _reference-dic-tags-serializer-normalizer:

serializer.normalizer
---------------------

**目的**: ``serializer`` サービスに独自のノーマライザーを追加します

このタグをつけるクラスは :class:`Symfony\\Component\\Serializer\\Normalizer\\NormalizerInterface` と :class:`Symfony\\Component\\Serializer\\Normalizer\\DenormalizerInterface` を実装している必要があります。

詳細は :doc:`/cookbook/serializer` を参照してください。

swiftmailer.plugin
------------------

**目的**: SwiftMailer のプラグインを追加します。

SwiftMailer の独自プラグインを使用している（または作成して使いたい）場合は ``swiftmailer.plugin`` タグ（オプションは何もありません）をつけてサービスを登録することで SwiftMailer のプラグインとして使うことができます。

SwiftMailer のプラグインは ``Swift_Events_EventListener`` を実装しなければなりません。
プラグインについて詳細は `SwiftMailer's Plugin Documentation`_ を参照してください。

いくつかの SwiftMailer プラグインは、 Symfony のコアに含まれており、別の設定で有効化することができます。詳細は :doc:`/reference/configuration/swiftmailer` を参照してください。

templating.helper
-----------------

**目的**: サービスをPHPテンプレートエンジンでヘルパーとして利用できるようにする

独自のテンプレートヘルパーを使うには、 ``templating.helper`` タグをつけてサービスとして登録し、 ``alias`` 属性を定義してください（ヘルパーは、テンプレートから ``alias`` を使って呼び出すことができます）。

.. configuration-block::

    .. code-block:: yaml

        services:
            templating.helper.your_helper_name:
                class: Fully\Qualified\Helper\Class\Name
                tags:
                    - { name: templating.helper, alias: alias_name }

    .. code-block:: xml

        <service id="templating.helper.your_helper_name" class="Fully\Qualified\Helper\Class\Name">
            <tag name="templating.helper" alias="alias_name" />
        </service>

    .. code-block:: php

        $container
            ->register('templating.helper.your_helper_name', 'Fully\Qualified\Helper\Class\Name')
            ->addTag('templating.helper', array('alias' => 'alias_name'))
        ;

.. _dic-tags-translation-loader:

translation.loader
------------------

**目的**: 独自の翻訳ローダーを追加します

翻訳は通常、ファイルシステムから様々な形式（YAML, XLIFF, PHPなど）で読み込まれます。
他のソースから翻訳を読み込ませたい場合は、まず :class:`Symfony\\Component\\Translation\\Loader\\LoaderInterface` を実装したクラスを作成します。::

    // src/Acme/MainBundle/Translation/MyCustomLoader.php
    namespace Acme\MainBundle\Translation;

    use Symfony\Component\Translation\Loader\LoaderInterface;
    use Symfony\Component\Translation\MessageCatalogue;

    class MyCustomLoader implements LoaderInterface
    {
        public function load($resource, $locale, $domain = 'messages')
        {
            $catalogue = new MessageCatalogue($locale);

            // 何かのリソースから翻訳を読み込み、カタログにセットします
            $catalogue->set('hello.world', 'Hello World!', $domain);

            return $catalogue;
        }
    }

カスタムローダーの ``load`` メソッドは :Class:`Symfony\\Component\\Translation\\MessageCatalogue` を返す必要があります。

次に、サービスとして登録し、 ``translation.loader`` タグをつけてください。

.. configuration-block::

    .. code-block:: yaml

        services:
            main.translation.my_custom_loader:
                class: Acme\MainBundle\Translation\MyCustomLoader
                tags:
                    - { name: translation.loader, alias: bin }

    .. code-block:: xml

        <service id="main.translation.my_custom_loader" class="Acme\MainBundle\Translation\MyCustomLoader">
            <tag name="translation.loader" alias="bin" />
        </service>

    .. code-block:: php

        $container
            ->register('main.translation.my_custom_loader', 'Acme\MainBundle\Translation\MyCustomLoader')
            ->addTag('translation.loader', array('alias' => 'bin'))
        ;

``alias`` オプションは必須で、とても重要です。このローダーが読み込むファイルのエクステンション子として使われるからです。
たとえば、翻訳ファイルとして読み込ませたい ``bin`` フォーマットがあるとします。
``messages`` ドメインのフランス語の翻訳メッセージを含む ``bin`` ファイルは、 ``app/Resources/translations/messages.fr.bin`` になるでしょう。

Symfony が ``bin`` ファイルを読み込むとき、カスタムローダーには翻訳ファイルのパスが ``$resource`` として渡されます。
カスタムローダーでは、翻訳メッセージの読み込むのに必要な処理を自由に実行することができます。

もしデータベースから翻訳メッセージを読み込む場合であっても、リソースファイルが必要ですが、空のファイルやデータベースから読み込む旨の簡単な記述だけでも構いません。
ファイルは、カスタムローダーの ``load`` メソッドを動作させるための鍵になっているのです。

translation.extractor
---------------------

**目的**: テンプレートファイルから翻訳が必要なメッセージを展開する独自サービスを追加します

``translation:update`` コマンドを実行したとき、ファイルから翻訳が必要なメッセージを展開するのにextractorが使われます。
Symfony2 フレームワークは通常、 :class:`Symfony\\Bridge\\Twig\\Translation\\TwigExtractor` と :class:`Symfony\\Bundle\\FrameworkBundle\\Translation\\PhpExtractor` を使用して Twig テンプレートや PHP ファイルから翻訳が必要なメッセージを抽出して展開しています。

:class:`Symfony\\Component\\Translation\\Extractor\\ExtractorInterface` を実装したクラスを作成し、 ``translation.extractor`` タグをつけることで独自のextractorを作成することができます。タグには ``alias`` オプションが必要で、extractorの名前として使われます。::

    // src/Acme/DemoBundle/Translation/FooExtractor.php
    namespace Acme\DemoBundle\Translation;

    use Symfony\Component\Translation\Extractor\ExtractorInterface;
    use Symfony\Component\Translation\MessageCatalogue;

    class FooExtractor implements ExtractorInterface
    {
        protected $prefix;

        /**
         * テンプレートディレクトリから翻訳が必要なメッセージを抽出し、MessageCatalogueに入れます
         */
        public function extract($directory, MessageCatalogue $catalogue)
        {
            // ...
        }

        /**
         * 新しく発見されたメッセージに使用するプリフィックスを設定します
         */
        public function setPrefix($prefix)
        {
            $this->prefix = $prefix;
        }
    }

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_demo.translation.extractor.foo:
                class: Acme\DemoBundle\Translation\FooExtractor
                tags:
                    - { name: translation.extractor, alias: foo }

    .. code-block:: xml

        <service id="acme_demo.translation.extractor.foo"
            class="Acme\DemoBundle\Translation\FooExtractor">
            <tag name="translation.extractor" alias="foo" />
        </service>

    .. code-block:: php

        $container->register(
            'acme_demo.translation.extractor.foo',
            'Acme\DemoBundle\Translation\FooExtractor'
        )
            ->addTag('translation.extractor', array('alias' => 'foo'));

translation.dumper
------------------

**目的**: 翻訳メッセージをダンプする独自サービスを追加します

`Extractor <translation.extractor>`_ がテンプレートからすべてのメッセージを展開し終わった後、特別なフォーマットで翻訳メッセージをダンプするためにdumperが実行されます。

Symfony2 には既にたくさんのdumperが含まれています。

* :class:`Symfony\\Component\\Translation\\Dumper\\CsvFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\IcuResFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\IniFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\MoFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\PoFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\QtFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\XliffFileDumper`
* :class:`Symfony\\Component\\Translation\\Dumper\\YamlFileDumper`

:class:`Symfony\\Component\\Translation\\Dumper\\FileDumper` を継承するか、 :class:`Symfony\\Component\\Translation\\Dumper\\DumperInterface` を実装して ``translation.dumper`` タグをつけることで dumper を追加することができます。 ``alias`` は必須のオプションで、この名前はどの dumper を使うべきか決定する際に使用されます。

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_demo.translation.dumper.json:
                class: Acme\DemoBundle\Translation\JsonFileDumper
                tags:
                    - { name: translation.dumper, alias: json }

    .. code-block:: xml

        <service id="acme_demo.translation.dumper.json"
            class="Acme\DemoBundle\Translation\JsonFileDumper">
            <tag name="translation.dumper" alias="json" />
        </service>

    .. code-block:: php

        $container->register(
            'acme_demo.translation.dumper.json',
            'Acme\DemoBundle\Translation\JsonFileDumper'
        )
            ->addTag('translation.dumper', array('alias' => 'json'));

.. _reference-dic-tags-twig-extension:

twig.extension
--------------

**目的**: 独自のTwigエクステンションを追加します

Twigエクステンションを使うには、サービスとして登録して ``twig.extension`` タグをつけます。

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.your_extension_name:
                class: Fully\Qualified\Extension\Class\Name
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <service id="twig.extension.your_extension_name" class="Fully\Qualified\Extension\Class\Name">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $container
            ->register('twig.extension.your_extension_name', 'Fully\Qualified\Extension\Class\Name')
            ->addTag('twig.extension')
        ;

実際に Twig のエクステンションクラスを作成する方法については `Twig のドキュメント`_ や :doc:`/cookbook/templating/twig_extension` を参照してください。

独自の Twig エクステンションを書く前に、 `Twig 公式エクステンションレポジトリ`_ をチェックしてください。役に立つエクステンションが登録されています。
たとえば ``Intl`` エクステンションや、 ``Intl`` に含まれるロケールに従って日付をフォーマットする ``localizeddate`` フィルタなどです。
Twig の公式エクステンションであっても、通常通りサービスとして登録する必要があります。

.. configuration-block::

    .. code-block:: yaml

        services:
            twig.extension.intl:
                class: Twig_Extensions_Extension_Intl
                tags:
                    - { name: twig.extension }

    .. code-block:: xml

        <service id="twig.extension.intl" class="Twig_Extensions_Extension_Intl">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $container
            ->register('twig.extension.intl', 'Twig_Extensions_Extension_Intl')
            ->addTag('twig.extension')
        ;

twig.loader
-----------

**目的**: Twig テンプレートを読み込む独自サービスを追加します

Symfony は通常、 `Twig Loader`_ つまり :class:`Symfony\\Bundle\\TwigBundle\\Loader\\FilesystemLoader` だけを使います。
他のリソースからテンプレートを読み込ませる必要がある場合は、新しいローダーを作成して ``twig.loader`` タグをつけてください。

.. configuration-block::

    .. code-block:: yaml

        services:
            acme.demo_bundle.loader.some_twig_loader:
                class: Acme\DemoBundle\Loader\SomeTwigLoader
                tags:
                    - { name: twig.loader }

    .. code-block:: xml

        <service id="acme.demo_bundle.loader.some_twig_loader" class="Acme\DemoBundle\Loader\SomeTwigLoader">
            <tag name="twig.loader" />
        </service>

    .. code-block:: php

        $container
            ->register('acme.demo_bundle.loader.some_twig_loader', 'Acme\DemoBundle\Loader\SomeTwigLoader')
            ->addTag('twig.loader')
        ;

validator.constraint_validator
------------------------------

**目的**: 独自のバリデーション制約を追加します

このタグを使うと、独自のバリデーション制約を追加して登録することができます。
詳しくは、  :doc:`/cookbook/validation/custom_constraint` を参照してください。

validator.initializer
---------------------

**目的**: バリデーション前にオブジェクトを初期化する独自サービスを追加します

このタグを使うと、バリデーションの直前に何かの処理を加えるという極めて稀な挙動を追加することができます。
たとえば、 Doctrine はこの機能を使って、遅延ロードすべきデータをバリデーション前のオブジェクトに追加しています。
initializerがなければ、 Doctrine のエンティティに含まれるデータが現実には存在していても、バリデーション時には存在しないように見えてしまいます。

本当にこの機能が必要なら、 :class:`Symfony\\Component\\Validator\\ObjectInitializerInterface` を実装してクラスを作成し、 ``validator.initializer`` タグをつけてください。このタグにオプションはありません。

一例として、Doctrine Bridge に含まれる ``EntityInitializer`` クラスがあります。

.. _`Twig のドキュメント`: http://twig.sensiolabs.org/doc/advanced.html#creating-an-extension
.. _`Twig 公式エクステンションレポジトリ`: https://github.com/fabpot/Twig-extensions
.. _`KernelEvents`: https://github.com/symfony/symfony/blob/2.2/src/Symfony/Component/HttpKernel/KernelEvents.php
.. _`SwiftMailer's Plugin Documentation`: http://swiftmailer.org/docs/plugins.html
.. _`Twig Loader`: http://twig.sensiolabs.org/doc/api.html#loaders


.. 2013/11/23 77web b9b60d763ff6030713246b6977d8b4e61f9c767c
