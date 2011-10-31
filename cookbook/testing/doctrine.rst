.. index::
   single: Tests; Doctrine

Doctrine のリポジトリクラスをテストする方法
===========================================

Symfony プロジェクトで Doctrine のリポジトリクラスをテストするのは、単純ではありません。
実際、リポジトリをロードするにはエンティティ、エンティティマネージャー、およびデータベース接続などいくつかのものをロードしたり準備しておく必要があります。

リポジトリクラスのテストには、次の 2 種類のアプローチがあります。

1) **ファンクショナルテスト**: このアプローチでは、実際のデータベース接続を使い、データベースの実際のデータを使います。
   ですのでセットアップして何かテストを行うことは簡単ですが、テストの実行速度が遅くなります。
   :ref:`cookbook-doctrine-repo-functional-test` も参照してください。

2) **Unit テスト**: Unit テストは、より実行速度が速く、テストしたいことを正確に実行できます。
   Unit テストを実行するには、このページで説明するように多少のセットアップが必要になります。
   また、リポジトリクラスの Unit テストでは、たとえばクエリーのビルドといったメソッドのみをテストし、それらを実際に実行するメソッドのテストは行いません。

Unit テスト
-----------

Symfony と Doctrine では共通のテスティングフレームワークを使っていますので、Unit テストを Symfony プロジェクト内で記述するのはとても簡単です。
ORM には、Unit テストや必要なオブジェクトのモックを簡単に作成するためのツールが備わっています。
たとえば、接続オブジェクト（Connection）やエンティティマネージャーオブジェクト（EntityManager）のモッキングなどです。
Doctrine で提供されているテストコンポーネントを使い、基本的なセットアップを行うと、リポジトリクラスの Unit テストに Doctrine のツールを広く活用できます。

実際のクエリの実行をテストしたい場合は、ファンクショナルテスト(:ref:`cookbook-doctrine-repo-functional-test` を参照)を行う必要があることに注意してください。
リポジトリクラスの Unit テストでは、クエリーをビルドするメソッドのみテストが可能です。

セットアップ
~~~~~~~~~~~~

最初に、オートローダーの設定に Doctrine\Tests 名前空間を追加します。

::

    // app/autoload.php
    $loader->registerNamespaces(array(
        //...
        'Doctrine\\Tests'                => __DIR__.'/../vendor/doctrine/tests',
    ));

次に、Doctrine からエンティティやリポジトリを読み込めるように、エンティティマネージャーをセットアップします。

Doctrine は、デフォルトではエンティティのアノテーションメタデータを読み込めませんので、アノテーションリーダーの設定を行い、エンティティのパースと読み込みを行えるようにします。

::

    // src/Acme/ProductBundle/Tests/Entity/ProductRepositoryTest.php
    namespace Acme\ProductBundle\Tests\Entity;

    use Doctrine\Tests\OrmTestCase;
    use Doctrine\Common\Annotations\AnnotationReader;
    use Doctrine\ORM\Mapping\Driver\DriverChain;
    use Doctrine\ORM\Mapping\Driver\AnnotationDriver;

    class ProductRepositoryTest extends OrmTestCase
    {
        private $_em;

        protected function setUp()
        {
            $reader = new AnnotationReader();
            $reader->setIgnoreNotImportedAnnotations(true);
            $reader->setEnableParsePhpImports(true);

            $metadataDriver = new AnnotationDriver(
                $reader, 
                // provide the namespace of the entities you want to tests
                'Acme\\ProductBundle\\Entity'
            );

            $this->_em = $this->_getTestEntityManager();

            $this->_em->getConfiguration()
            	->setMetadataDriverImpl($metadataDriver);

            // AcmeProductBundle:Product 形式を使えるようにする
            $this->_em->getConfiguration()->setEntityNamespaces(array(
                'AcmeProductBundle' => 'Acme\\ProductBundle\\Entity'
            ));
        }
    }

上のコードを見ると、次のようなことに気づくでしょう:

* テストクラスは、Unit テスト用の便利なメソッドが実装されている ``\Doctrine\Tests\OrmTestCase`` を継承します。

* ``AnnotationReader`` をセットアップして、エンティティのパースと読み込みを行えるようにしています。

* ``_getTestEntityManager`` メソッドを呼び出してエンティティマネージャーを取得しています。
  このメソッドで取得できるのは、モック化された接続オブジェクトを使う、モック化されたエンティティマネージャーです。

セットアップはこれだけです。これで Doctrine のリポジトリクラスの Unit テストを記述する準備が整いました。

Unit テストの記述
~~~~~~~~~~~~~~~~~

Doctrine のリポジトリクラスのメソッドで、クエリーをビルドし、その場でクエリーを実行するのではなく、ビルドしたクエリーを返すようなメソッドのみ Unit テストが可能だということを思い出してください。
クエリーをビルドして返すメソッドとは、たとえば次のようなコードになります。

::

    // src/Acme/StoreBundle/Entity/ProductRepository
    namespace Acme\StoreBundle\Entity;

    use Doctrine\ORM\EntityRepository;

    class ProductRepository extends EntityRepository
    {
        public function createSearchByNameQueryBuilder($name)
        {
            return $this->createQueryBuilder('p')
                ->where('p.name LIKE :name', $name)
        }
    }

この例では、メソッドは ``QueryBuilder`` インスタンスを返しています。
このメソッドの結果は、さまざまな方法でテストできるでしょう。

::

    class ProductRepositoryTest extends \Doctrine\Tests\OrmTestCase
    {
        /* ... */

        public function testCreateSearchByNameQueryBuilder()
        {
            $queryBuilder = $this->_em->getRepository('AcmeProductBundle:Product')
                ->createSearchByNameQueryBuilder('foo');

            $this->assertEquals('p.name LIKE :name', (string) $queryBuilder->getDqlPart('where'));
            $this->assertEquals(array('name' => 'foo'), $queryBuilder->getParameters());
        }
     }

このテストコードでは、返された ``QueryBuilder`` オブジェクトを調べて、クエリーの各パーツが期待した内容になっていることを確認しています。
クエリービルダーで他に追加する可能性のある SQL パーツとしては、\ ``select``\ 、\ ``from``\ 、\ ``join``\ 、\ ``set``\ 、\ ``groupBy``\ 、\ ``having``\ 、\ ``orderBy`` などがあります。

メソッドから返されるのが ``Query`` オブジェクトであったり、実際のクエリーのみをテストしたい場合には、次のようにして直接 DQL のクエリー文字列をテストすることもできます。

::

    public function testCreateSearchByNameQueryBuilder()
    {
        $queryBuilder = $this->_em->getRepository('AcmeProductBundle:Product')
            ->createSearchByNameQueryBuilder('foo');

        $query = $queryBuilder->getQuery();

        // DQL 文字列をテストする
        $this->assertEquals(
            'SELECT p FROM Acme\ProductBundle\Entity\Product p WHERE p.name LIKE :name',
            $query->getDql()
        );
    }

.. _cookbook-doctrine-repo-functional-test:

ファンクショナルテスト
----------------------

実際にクエリーを実行した結果をテストする必要がある場合は、Symfony のカーネルを boot して実際のデータベース接続を取得します。
この場合は、Unit テストの時とは異なり、テストクラスで ``WebTestCase`` を継承します。
WebTestCase を使うと、テストコード内で Symfony のカーネルの boot などを簡単に行えます。

::

    // src/Acme/ProductBundle/Tests/Entity/ProductRepositoryFunctionalTest.php
    namespace Acme\ProductBundle\Tests\Entity;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class ProductRepositoryFunctionalTest extends WebTestCase
    {
        /**
         * @var \Doctrine\ORM\EntityManager
         */
        private $_em;

        public function setUp()
        {
        	$kernel = static::createKernel();
        	$kernel->boot();
            $this->_em = $kernel->getContainer()
                ->get('doctrine.orm.entity_manager');
        }

        public function testProductByCategoryName()
        {
            $results = $this->_em->getRepository('AcmeProductBundle:Product')
                ->searchProductsByNameQuery('foo')
                ->getResult();

            $this->assertEquals(count($results), 1);
        }
    }

.. 2011/10/30 hidenorigoto f22af2069e7085b4c6ef2599f3f2ca6e152e4cb8

