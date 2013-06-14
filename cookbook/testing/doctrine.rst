.. index::
   single: Tests; Doctrine

Doctrine のリポジトリクラスをテストする方法
===========================================

.. note::

    * 対象バージョン：2.0以降
    * 翻訳更新日：2013/6/14

Symfony プロジェクトで Doctrine のリポジトリクラスに対してユニットテストを記述することは推奨ではありません。リポジトリを操作する場合、通常は実際のデータベース接続に対してテストされることを意図しているからです。
リポジトリクラスのメソッドに何らかの特殊なロジックが実装されている場合は、その部分のみユニットテストの対象とします。

このページでは、実際のデータベースへのクエリーをテストする方法を解説しています。

.. _cookbook-doctrine-repo-functional-test:

ファンクショナルテスト
----------------------

実際にクエリーを実行した結果をテストする必要がある場合は、Symfony のカーネルを boot して実際のデータベース接続を取得します。
テストクラスで ``WebTestCase`` を継承します。
WebTestCase を使うと、テストコード内で Symfony のカーネルの boot などを簡単に行えます。

.. code-block:: php

    // src/Acme/StoreBundle/Tests/Entity/ProductRepositoryFunctionalTest.php
    namespace Acme\StoreBundle\Tests\Entity;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class ProductRepositoryFunctionalTest extends WebTestCase
    {
        /**
         * @var \Doctrine\ORM\EntityManager
         */
        private $em;

        /**
         * {@inheritDoc}
         */
        public function setUp()
        {
            static::$kernel = static::createKernel();
            static::$kernel->boot();
            $this->em = static::$kernel->getContainer()
                ->get('doctrine')
                ->getManager()
            ;
        }

        public function testSearchByCategoryName()
        {
            $products = $this->em
                ->getRepository('AcmeStoreBundle:Product')
                ->searchByCategoryName('foo')
            ;

            $this->assertCount(1, $products);
        }

        /**
         * {@inheritDoc}
         */
        protected function tearDown()
        {
            parent::tearDown();
            $this->em->close();
        }
    }

.. 2011/10/30 hidenorigoto f22af2069e7085b4c6ef2599f3f2ca6e152e4cb8
.. 2013/06/14 hidenorigoto a03f06f7657a29fbfc8a049da1d92ab9e1681624
