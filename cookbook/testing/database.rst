.. 2013/10/10 monmonmon 899d0f0d9964aeda17b0716bd772eb75cb304da5

.. index::
   single: Tests; Database

.. note::

    * 対象バージョン：2.3 (2.0以降)
    * 翻訳更新日：2013/10/10

データベース接続を伴うコードのテストについて
============================================

データベースとやり取りするコードをテストする場合、それに合わせたテストを書く必要があります。
ユニットテストでは、リポジトリのモックを書いてデータベース接続をシミュレートすることが出来るでしょう。
ファンクショナルテストでは、毎回同じデータでテスト出来るようにテストデータベースを用意する必要があるでしょう。

.. note::

    SQL クエリを直接テストする場合は :doc:`/cookbook/testing/doctrine` をご覧下さい。

リポジトリのモックを用いたユニットテスト
---------------------------------------------

データベース接続をリポジトリのみで行うコードであれば、リポジトリのモックを作ってテストを行います。
リポジトリは通常 ``EntityManager`` を通じて取得するため、\ ``EntityManager`` のモックも作成する必要があります。

.. tip::

    リポジトリをファクトリ・サービス (:doc:`</components/dependency_injection/factories>`) として登録することで、\ ``EntityManager`` を介さずに直接リポジトリを取得することが出来ます。
    多少の準備が必要ですが、リポジトリのモックを作るだけで済むようになります。

このようなクラスをテストするとします。::

    namespace Acme\DemoBundle\Salary;

    use Doctrine\Common\Persistence\ObjectManager;

    class SalaryCalculator
    {
        private $entityManager;

        public function __construct(ObjectManager $entityManager)
        {
            $this->entityManager = $entityManager;
        }

        public function calculateTotalSalary($id)
        {
            $employeeRepository = $this->entityManager->getRepository('AcmeDemoBundle::Employee');
            $employee = $userRepository->find($id);

            return $employee->getSalary() + $employee->getBonus();
        }
    }

コンストラクタ引数で ``ObjectManager`` オブジェクトを渡すようになっているため、テストではモックオブジェクトを作って渡すだけで済みます。::

    use Acme\DemoBundle\Salary\SalaryCalculator;

    class SalaryCalculatorTest extends \PHPUnit_Framework_TestCase
    {

        public function testCalculateTotalSalary()
        {
            // テストで用いる employee のモックを生成
            $employee = $this->getMock('\Acme\DemoBundle\Entity\Employee');
            $employee->expects($this->once())
                ->method('getSalary')
                ->will($this->returnValue(1000));
            $employee->expects($this->once())
                ->method('getBonus')
                ->will($this->returnValue(1100));

            // employee を返すリポジトリのモックを生成
            $employeeRepository = $this->getMockBuilder('\Doctrine\ORM\EntityRepository')
                ->disableOriginalConstructor()
                ->getMock();
            $employeeRepository->expects($this->once())
                ->method('find')
                ->will($this->returnValue($employee));

            // リポジトリを返す EntityManager のモックを生成
            $entityManager = $this->getMockBuilder('\Doctrine\Common\Persistence\ObjectManager')
                ->disableOriginalConstructor()
                ->getMock();
            $entityManager->expects($this->once())
                ->method('getRepository')
                ->will($this->returnValue($employeeRepository));

            $salaryCalculator = new SalaryCalculator($entityManager);
            $this->assertEquals(2100, $salaryCalculator->calculateTotalSalary(1));
        }
    }

この例では、完全にモックのみでテストを行っています。
``Employee`` エンティティもそれを返すリポジトリも、更にそれを生成する ``Entitymanager`` も、全てモックです。本物のクラスは使われていません。

ファンクショナルテスト用にデータベースを設定
--------------------------------------------

ファンクショナルテストの場合は、実際のデータベースとのやり取りが必要になるでしょう。
開発用データを誤って上書きしたりしないよう、またテスト毎にデータをクリア出来るよう、
多くの場合テスト専用のデータベースを用意することになります。

このために、テスト用のコンフィギュレーションファイルを書いて通常のデータベース設定をオーバーライドします。

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        doctrine:
            # ...
            dbal:
                host: localhost
                dbname: testdb
                user: testdb
                password: testdb

    .. code-block:: xml

        <!-- app/config/config_test.xml -->
        <doctrine:config>
            <doctrine:dbal
                host="localhost"
                dbname="testdb"
                user="testdb"
                password="testdb"
            />
        </doctrine:config>

    .. code-block:: php

        // app/config/config_test.php
        $configuration->loadFromExtension('doctrine', array(
            'dbal' => array(
                'host'     => 'localhost',
                'dbname'   => 'testdb',
                'user'     => 'testdb',
                'password' => 'testdb',
            ),
        ));

テストを実行する前に、テスト用のデータベースやユーザーが作成済みであることを確認しましょう。
