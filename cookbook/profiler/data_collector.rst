.. index::
   single: Profiling; Data Collector

カスタムデータコレクタの作成方法
=====================================

Symfony2 の  :doc:`Profiler </book/internals/profiler>` はデータの収集をデータコレクタに移譲します。 Symfony2 にはいくつかデータコレクタが付属していますが、簡単に自分自身で作成することもできます。

カスタムデータコレクタを作成する
--------------------------------

カスタムデータコレクタを作成するのは、 :class:`Symfony\\Component\\HttpKernel\\DataCollector\\DataCollectorInterface` を実装するだけで、非常に簡単です。
::

    interface DataCollectorInterface
    {
        /**
         * Collects data for the given Request and Response.
         *
         * @param Request    $request   A Request instance
         * @param Response   $response  A Response instance
         * @param \Exception $exception An Exception instance
         */
        function collect(Request $request, Response $response, \Exception $exception = null);

        /**
         * Returns the name of the collector.
         *
         * @return string The collector name
         */
        function getName();
    }

``getName()`` メソッドは、ユニークな名前を返す必要があります。この名前は、 :doc:`/cookbook/testing/profiling` などで情報へのアクセスとして使用されます。

``collect()`` メソッドは、ローカルのプロパティでアクセスさせたいデータを格納する役割を担います。

.. caution::

    プロファイラは、データコレクタのインスタンスをシリアライズするので、 PDO オブジェクトのようなシリアライズができないオブジェクトを格納すべきではありません。もし、そのような際には、自分で ``serialize()`` メソッドを用意する必要があります。

ほとんどの場合において ``$this->data`` プロパティのシリアライズを面倒みてくれる :class:`Symfony\\Component\\HttpKernel\\DataCollector\\DataCollector` を拡張して、 ``$this->data`` プロパティに追加することができるので便利です。
::

    class MemoryDataCollector extends DataCollector
    {
        public function collect(Request $request, Response $response, \Exception $exception = null)
        {
            $this->data = array(
                'memory' => memory_get_peak_usage(true),
            );
        }

        public function getMemory()
        {
            return $this->data['memory'];
        }

        public function getName()
        {
            return 'memory';
        }
    }

.. _data_collector_tag:

カスタムデータコレクタを有効にする
-------------------------------

データコレクタを有効にするには、コンフィギュレーションのどこかでレギュラーサービスとして追加して、 ``data_collector`` をタグ付けしてください。

.. configuration-block::

    .. code-block:: yaml

        services:
            data_collector.your_collector_name:
                class: Fully\Qualified\Collector\Class\Name
                tags:
                    - { name: data_collector }

    .. code-block:: xml

        <service id="data_collector.your_collector_name" class="Fully\Qualified\Collector\Class\Name">
            <tag name="data_collector" />
        </service>

    .. code-block:: php

        $container
            ->register('data_collector.your_collector_name', 'Fully\Qualified\Collector\Class\Name')
            ->addTag('data_collector')
        ;

ウェブプロファイラテンプレートを追加する
-----------------------------

ウェブデバッグツールバーやウェブプロファイラで独自のデータコレクタで集めたデータを表示したい際には、 以下のスケルトンで Twig テンプレートを作成してください。

.. code-block:: jinja

    {% extends 'WebProfilerBundle:Profiler:layout.html.twig' %}

    {% block toolbar %}
        {# the web debug toolbar content #}
    {% endblock %}

    {% block head %}
        {# if the web profiler panel needs some specific JS or CSS files #}
    {% endblock %}

    {% block menu %}
        {# the menu content #}
    {% endblock %}

    {% block panel %}
        {# the panel content #}
    {% endblock %}

それぞれのブロックは、オプションです。 ``toolbar`` ブロックは、ウェブデバッグツールバーで使用され、 ``menu`` と ``panel`` はウェブプロファイラにパネルを追加するのに使われます。

すべてのブロックは ``collector`` オブジェクトへのアクセス権を持っています。

.. tip::

    ビルトインしているテンプレートは、ツールバーの画像に base64 エンコードを使用しています(``<img src="src="data:image/png;base64,..."``)。画像の base64 の値は次の小さなスクリプトで簡単に調べることができます。:  ``echo base64_encode(file_get_contents($_SERVER['argv'][1]));``


テンプレートを有効にするには、コンフィギュレーションに ``template`` 属性を ``data_collector`` タグに追加してください。例として、 ``AcmeDebugBundle`` にテンプレートああるとすると、次のようになります。

.. configuration-block::

    .. code-block:: yaml

        services:
            data_collector.your_collector_name:
                class: Acme\DebugBundle\Collector\Class\Name
                tags:
                    - { name: data_collector, template: "AcmeDebug:Collector:templatename", id: "your_collector_name" }

    .. code-block:: xml

        <service id="data_collector.your_collector_name" class="Acme\DebugBundle\Collector\Class\Name">
            <tag name="data_collector" template="AcmeDebug:Collector:templatename" id="your_collector_name" />
        </service>

    .. code-block:: php

        $container
            ->register('data_collector.your_collector_name', 'Acme\DebugBundle\Collector\Class\Name')
            ->addTag('data_collector', array('template' => 'AcmeDebugBundle:Collector:templatename', 'id' => 'your_collector_name'))
        ;

.. 2011/11/17 ganchiku f4cb10b3d506af39588d94409f13686f6b9e18da

