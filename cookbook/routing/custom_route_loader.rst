.. index::
   single: Routing; Custom route loader

.. note::

    * 対象バージョン : 2.3+
    * 翻訳更新日 : 2013/11/23

カスタムルートローダーの作り方
===================================

カスタムルートローダーを使うと、たとえば Yaml のような設定ファイルを読み込むことなくルーティング設定をアプリケーションに読み込ませることができます。
バンドル用のルーティングを ``app/config/routing.yml`` に手動で追加するのが煩わしいときに便利です。バンドルのルーティング設定を手動で更新するとインストール時に必要な作業を増やしたり、間違いを起こしやすくなったりするので、再利用可能なバンドルを作るときやオープンソースのバンドルを作るときに、カスタムルートローダーは特に重要になります。

それだけでなく、ルーティング設定を自動で生成させたり、何らかの規則やパターンに従って自動で配置させたりしたいときにも、カスタムルートローダーは役立ちます。
一例として、コントローラのアクションメソッド名を使ってルーティング設定が自動生成される `FOSRestBundle`_ が挙げられます。

.. note::

    上で説明されているようなケースに対応するためにカスタムルートローダーを利用しているバンドルはたくさんあります。
    たとえば、 `FOSRestBundle`_, `KnpRadBundle`_, `SonataAdminBundle`_ です。

ルーティング設定を読み込む
---------------------------

Symfony アプリケーションのルーティング設定は、 :class:`Symfony\\Bundle\\FrameworkBundle\\Routing\\DelegatingLoader` を使って読み込まれています。
このローダーは複数の違うローダーを使って（デリゲートして）、さまざまな種類のリソースから読み込みます。たとえば Yaml ファイルや、コントローラのファイル内の ``@Route`` ・ ``@Method`` アノテーションなどです。
特別のローダーは :class:`Symfony\\Component\\Config\\Loader\\LoaderInterface` を実装し、2つの重要なメソッドを備えています。
:method:`Symfony\\Component\\Config\\Loader\\LoaderInterface::supports` メソッドと :method:`Symfony\\Component\\Config\\Loader\\LoaderInterface::load` メソッドです。

Symfony standard edition の AcmeDemoBundle の ``routing.yml`` にあるこの記述を見てください。

.. code-block:: yaml

    # src/Acme/DemoBundle/Resources/config/routing.yml
    _demo:
        resource: "@AcmeDemoBundle/Controller/DemoController.php"
        type:     annotation
        prefix:   /demo


メインのローダーがこの記述をパースすると、与えられたリソース（ ``@AcmeDemoBundle/Controller/DemoController.php`` ）とタイプ（ ``annotation`` ）についてすべてのローダーの :method:`Symfony\\Component\\Config\\Loader\\LoaderInterface::supports` メソッドを呼び出します。
ローダーのどれかが ``true`` を返すと、そのローダーの :method:`Symfony\\Component\\Config\\Loader\\LoaderInterface::load` メソッドが呼び出され、 ::class:`Symfony\\Component\\Routing\\Route` を含む class:`Symfony\\Component\\Routing\\RouteCollection` が返されます。

カスタムローダーを作成する
----------------------------

ルーティング設定を独自のリソース（たとえば、アノテーション・ Yaml ファイル・ XML ファイル以外）から読み込ませるためには、カスタムルートローダーを作成する必要があります。
カスタムルートローダーは :class:`Symfony\\Component\\Config\\Loader\\LoaderInterface` を実装している必要があります。

下記のサンプルローダーは、 ``extra`` という種類のルーティングリソースからのルーティング設定読み込みをサポートしています。
``extra`` という種類は重要ではありません。好きなリソースを命名して使うことができます。リソース名自体はサンプルの中では使われていません。::

    namespace Acme\DemoBundle\Routing;

    use Symfony\Component\Config\Loader\LoaderInterface;
    use Symfony\Component\Config\Loader\LoaderResolverInterface;
    use Symfony\Component\Routing\Route;
    use Symfony\Component\Routing\RouteCollection;

    class ExtraLoader implements LoaderInterface
    {
        private $loaded = false;

        public function load($resource, $type = null)
        {
            if (true === $this->loaded) {
                throw new \RuntimeException('"extra" ローダーを2回以上読み込ませないで下さい');
            }

            $routes = new RouteCollection();

            // 新しいルートを用意します
            $pattern = '/extra/{parameter}';
            $defaults = array(
                '_controller' => 'AcmeDemoBundle:Demo:extra',
            );
            $requirements = array(
                'parameter' => '\d+',
            );
            $route = new Route($pattern, $defaults, $requirements);

            // 新しいルートをルーティングコレクションに追加します
            $routeName = 'extraRoute';
            $routes->add($routeName, $route);
            
            $this->loaded = true;

            return $routes;
        }

        public function supports($resource, $type = null)
        {
            return 'extra' === $type;
        }

        public function getResolver()
        {
            // このメソッドは必要ですが、他のリソースを読み込まない場合は空で構いません。
            // 他のリソースを読み込むときは Loader 基本クラスを利用すると便利です
        }

        public function setResolver(LoaderResolverInterface $resolver)
        {
            // 上に同じ
        }
    }

.. note::

    ルーティングに指定したコントローラーが実際に存在するのを確認してください。

次に ``ExtraLoader`` のサービスを定義します。

.. configuration-block::

    .. code-block:: yaml

        services:
            acme_demo.routing_loader:
                class: Acme\DemoBundle\Routing\ExtraLoader
                tags:
                    - { name: routing.loader }

    .. code-block:: xml

        <?xml version="1.0" ?>
        <container xmlns="http://symfony.com/schema/dic/services"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <services>
                <service id="acme_demo.routing_loader" class="Acme\DemoBundle\Routing\ExtraLoader">
                    <tag name="routing.loader" />
                </service>
            </services>
        </container>

    .. code-block:: php

        use Symfony\Component\DependencyInjection\Definition;

        $container
            ->setDefinition(
                'acme_demo.routing_loader',
                new Definition('Acme\DemoBundle\Routing\ExtraLoader')
            )
            ->addTag('routing.loader')
        ;

``routing.loader`` タグが使われています。このタグがついているすべてのサービスはルートローダーとして認識され、 :class:`Symfony\\Bundle\\FrameworkBundle\\Routing\\DelegatingLoader` にカスタムルートローダーとして登録されます。

カスタムローダーを使う
~~~~~~~~~~~~~~~~~~~~~~~

他に何もしなければ、カスタムルートローダーは *呼び出されることはありません。*
カスタムローダーを呼び出したければ、ルーティング設定に数行の設定を追加する必要があります。

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        AcmeDemoBundle_Extra:
            resource: .
            type: extra

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="." type="extra" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import('.', 'extra'));

        return $collection;

重要なのは ``type`` 設定です。"extra" になっていなければなりません。
"extra" は ``ExtraLoader`` がサポートしているリソースタイプであり、 ``ExtraLoader`` の ``load()`` メソッドを呼び出されるようになります。 ``resource`` 設定は ``ExtraLoader`` にとっては重要ではないので、 "." に設定してあります。

.. note::

    カスタムルートローダーで定義されたルーティング設定は Symfony が自動的にキャッシュします。
    ローダークラスの内容を変更したときは、キャッシュクリアを忘れずに実行してください。

より高度なローダー
---------------------

ほとんどの場合、 :class:`Symfony\\Component\\Config\\Loader\\LoaderInterface` を実装するより  :class:`Symfony\\Component\\Config\\Loader\\Loader` を継承したほうが便利です。
Loader クラスは :class:`Symfony\\Component\\Config\\Loader\\LoaderResolver` を使ってルーティングリソースを読み込む方法が組み込まれているからです。

と言っても :method:`Symfony\\Component\\Config\\Loader\\LoaderInterface::supports` メソッドと :method:`Symfony\\Component\\Config\\Loader\\LoaderInterface::load` メソッドは実装しなくてはいけませんが、他のリソース（たとえば Yaml のルーティング設定ファイル）を読み込みたいときに、 :method:`Symfony\\Component\\Config\\Loader\\Loader::import` メソッドを利用することができます。::

    namespace Acme\DemoBundle\Routing;

    use Symfony\Component\Config\Loader\Loader;
    use Symfony\Component\Routing\RouteCollection;

    class AdvancedLoader extends Loader
    {
        public function load($resource, $type = null)
        {
            $collection = new RouteCollection();

            $resource = '@AcmeDemoBundle/Resources/config/import_routing.yml';
            $type = 'yaml';

            $importedRoutes = $this->import($resource, $type);

            $collection->addCollection($importedRoutes);

            return $collection;
        }

        public function supports($resource, $type = null)
        {
            return $type === 'advanced_extra';
        }
    }

.. note::

    インポートするルーティング設定ファイルのリソース名とタイプは、ルートローダーで通常サポートされているどのタイプでも使用することができます。（Yaml, XML, PHP, アノテーションなど）

.. _`FOSRestBundle`: https://github.com/FriendsOfSymfony/FOSRestBundle
.. _`KnpRadBundle`: https://github.com/KnpLabs/KnpRadBundle
.. _`SonataAdminBundle`: https://github.com/sonata-project/SonataAdminBundle

.. 2013/11/23 77web 8313805c904f9b46a163ec9841a1c33443be2533
