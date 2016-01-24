.. index::
   single: Doctrine

データベースと Doctrine ("The Model")
=====================================

現実を認めよう。\
どんなアプリケーションにとっても、データベースへの情報の永続化やデータベースからの情報の取得は、\
最もよく使われ、そしてチャレンジしがいのあるタスクです。\
そのタスクを簡単にこなすことができる強力なツールを提供する、\
というただ一点の目的だけに特化した `Doctrine`_ というライブラリを、幸運なことにも Symfony は統合しています。\
この章では、Doctrine の基本的なフィロソフィと、どれだけ簡単にデータベースを使うことができるか、という点を見ていきます。

.. note::

    Doctrine 自体は、Symfony とは完全に独立していて、Symfony で使用することはオプションです。\
    この章は、オブジェクトとリレーショナルデータベース(*MySQL* や *PostgreSQL*\ 、\ *Microsoft SQL*)をマップする、\
    Doctrine ORM についての章です。\
    データベースに直接クエリを実行することも簡単で、クックブックの ":doc:`/cookbook/doctrine/dbal`" で説明しています。

    Doctrine ODM ライブラリを使って `MongoDB`_ に永続化することも可能です。\
    詳細は、"\ :doc:`/bundles/DoctrineMongoDBBundle/index`\ "を参照してください。

簡単な例: 商品
--------------

Doctrine の動作を理解するのに一番簡単な方法は、実際に動かしてみることでしょう。\
この節では、データベースの設定を行い、そして、\ ``Product(商品)`` オブジェクトを作成して、\
データベースへの永続化を行い、反対にそれをフェッチしてみます。

.. sidebar:: この章のサンプルコードを実行するには

    この章で出てくる例を実際に追っていくには、次のコマンドを実行して ``AcmeStoreBundle`` を作成しておいてください。
    
    .. code-block:: bash
    
        php app/console generate:bundle --namespace=Acme/StoreBundle

データベースの設定
~~~~~~~~~~~~~~~~~~

本当のスタート前に、まずは、データベース接続の設定を行う必要があります。\
慣例として、通常は ``app/config/parameters.ini`` で設定を行います。

.. code-block:: ini

    # app/config/parameters.ini
    parameters
        database_driver   = pdo_mysql
        database_host     = localhost
        database_name     = test_project
        database_user     = root
        database_password = password

.. note::

    ``parameters.ini`` を通して設定を行うのは、単なる慣習です。\
    このファイルで定義したパラメータは、Doctrine の設定時に、メインの設定ファイルから参照されます。
    
    .. code-block:: yaml
    
        doctrine:
            dbal:
                driver:   %database_driver%
                host:     %database_host%
                dbname:   %database_name%
                user:     %database_user%
                password: %database_password%
    
    データベース情報を、別々のファイルにしておくことによって、\
    各サーバ上で、ファイルを異なるバージョンで保持できるようになります。\
    データベースの設定(もしくは、センシティブな情報)は、簡単にプロジェクトの外部に置くことができます。\
    例えば、Apache 設定ファイル内に置くことも可能です。\
    詳細は、\ :doc:`/cookbook/configuration/external_parameters` を参照してください。


これで Doctrine からデータベースの情報を読み取れるようになったので、\
Doctrine からデータベースを作成してみましょう。次のコマンドを実行してください。

.. code-block:: bash

    php app/console doctrine:database:create

エンティティクラスの作成
~~~~~~~~~~~~~~~~~~~~~~~~

商品を表示するようなアプリケーションを作っているとしましょう。\
Doctrine やデータベース以前に、それら商品を表す ``Product`` オブジェクトが必要なのは、わかりきっていますよね。\
このクラスを ``AcmeStoreBundle`` 内の ``Entity`` ディレクトリ内に作成します。\ ::

    // src/Acme/StoreBundle/Entity/Product.php    
    namespace Acme\StoreBundle\Entity;

    class Product
    {
        protected $name;

        protected $price;

        protected $description;
    }

エンティティ(\ *データを保持する基本クラス*\ )と呼ばれることが多いですが、\
このクラスはシンプルで、アプリケーションで必要な商品のビジネス要件を満足させるクラスです。\
ただし、データベースに永続化することはできません。ただ単に PHP のクラスでしかありません。

.. tip::

    Doctrine の背景にあるコンセプトが理解できたので、\
    次のようにしても、エンティティクラスを生成できます。
    
    .. code-block:: bash
        
        php app/console doctrine:generate:entity --entity="AcmeStoreBundle:Product" --fields="name:string(255) price:float description:text"

.. index::
    single: Doctrine; Adding mapping metadata

.. _book-doctrine-adding-mapping:

マッピング情報の付加
~~~~~~~~~~~~~~~~~~~~

Doctrine では、単にカラムベースのテーブルを配列にしてフェッチするといったやり方よりも\
興味深いやり方で、データベースと連携することができます。\
オブジェクトそのものをデータベースに永続化して、データベースからオブジェクトそのものをフェッチしてくるのです。\
これは、PHP クラスをデータベーステーブルにマップし、\
クラスのプロパティをテーブルのカラムにマップすることで可能になります。


.. image:: /images/book/doctrine_image_1.png
   :align: center

Doctrine がこの作業をできるようにするには、"metadata（メタデータ）" を作成するか、\
``Product`` クラスとそのプロパティがデータベースにどの様にマップされるのか、という設定を作成するだけです。\
metadata は、色々なフォーマットで記述することが可能で、\
YAML や XML ファイルに記述するか、アノテーションとして ``Product`` クラスに直接記述することもできます。

.. note::

    １つのバンドル内では、metadata の定義には1つのフォーマットしか利用できません。\
    YAML による metadata の定義と、アノテーション付き PHP エンティティクラスのミックスといったことは不可能です。

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Entity/Product.php
        namespace Acme\StoreBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity
         * @ORM\Table(name="product")
         */
        class Product
        {
            /**
             * @ORM\Id
             * @ORM\Column(type="integer")
             * @ORM\GeneratedValue(strategy="AUTO")
             */
            protected $id;

            /**
             * @ORM\Column(type="string", length=100)
             */
            protected $name;

            /**
             * @ORM\Column(type="decimal", scale=2)
             */
            protected $price;

            /**
             * @ORM\Column(type="text")
             */
            protected $description;
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            table: product
            id:
                id:
                    type: integer
                    generator: { strategy: AUTO }
            fields:
                name:
                    type: string
                    length: 100
                price:
                    type: decimal
                    scale: 2
                description:
                    type: text

    .. code-block:: xml

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="Acme\StoreBundle\Entity\Product" table="product">
                <id name="id" type="integer" column="id">
                    <generator strategy="AUTO" />
                </id>
                <field name="name" column="name" type="string" length="100" />
                <field name="price" column="price" type="decimal" scale="2" />
                <field name="description" column="description" type="text" />
            </entity>
        </doctrine-mapping>

.. ** <- this is for vim hilighting problem, simply ignore this

.. tip::

    テーブル名はオプションです。省略された場合は、\
    エンティティクラス名に基づいて自動的にテーブル名が決定されます。

Doctrine には、広範囲の様々なフィールドタイプがあり、それらの内から選択できます。\
またそれぞれのフィールドには固有のオプションがあります。\
利用可能なフィールドタイプについては、\ :ref:`book-doctrine-field-types`\ を参照してください。

.. seealso::

    マッピングの詳細情報は、Doctrine のドキュメント `Basic Mapping Documentation`_ を参照してもよいでしょう。\
    Doctrineドキュメントの方には記載されていませんが、アノテーションを使用する場合は、\
    その先頭に ``ORM\`` を常に付加しておく必要があります(例 ``ORM\Column(..)``)。\
    同時に、\ ``use Doctrine\ORM\Mapping as ORM;`` も宣言しておく必要があります。\
    この宣言は、アノテーションのプリフィクス ``ORM`` を\ *インポート*\ するものです。

.. caution::

    クラス名やプロパティは、予約されたSQLキーワード(``group``\ や ``user``)にはマップされませんので、注意してください。\
    例えば、エンティティのクラス名が ``Group`` である場合、デフォルトでは、テーブル名が ``group`` となります。\
    しかし、このテーブル名は、いくつかのエンジンでは SQL エラーとなるでしょう。\
    Doctrine の `Reserved SQL keywords documentation`_ で、こういった名前をどうエスケープするか参照して下さい。

.. note::

    他のライブラリやプログラム(Doxygen) がアノテーションを使っている場合は、\
    クラスに ``@IgnoreAnnotation`` を付けて、Symfony がどのアノテーションを無視すべきかということを示してやる必要があります。

    例えば、\ ``@fn`` というアノテーションにより例外が投げられるのを防ぐには、次のようにします。\ ::

        /**
         * @IgnoreAnnotation("fn")
         */
        class Product

.. ** <- this is for vim hilighting problem, simply ignore this

ゲッターとセッターの作成
~~~~~~~~~~~~~~~~~~~~~~~~

さて、Doctrine が ``Product`` オブジェクトをデータベースにどうやって永続化するかがわかったとしても、\
今のところ、このクラス自体は全く便利ではありません。\
``Product`` は普通の PHP クラスで、そのプロパティにアクセスするには(プロパティが ``protected`` なので)、\
セッターやゲッターメソッドを作成する必要があります(例: ``getName()`` や ``setName()``)。\
ありがたい事に、次のコマンドを実行すると、これを Doctrine がやってくれます。


.. code-block:: bash

    php app/console doctrine:generate:entities Acme/StoreBundle/Entity/Product

このコマンドは、\ ``Product`` クラスにゲッターやセッターがすべて作成されているかを確認します。\
このコマンドは安全なコマンドで、何度でも実行が可能です。\
存在していないゲッターとセッターのみを作成してくれます(既存のメソッドを上書きすることはありません)。

.. caution::

    コマンド ``doctrine:generate:entities`` は、バックアップとして\ ``Product.php`` を ``Product.php~`` というファイル名で保存します。\
    このファイルの存在により、時に、"Cannot redeclare class" エラーが発生することがあります。\
    このファイルは安全に削除できます。

バンドル、もしくは名前空間内の既知のエンティティ(つまり、Doctrine のマップ情報がある全ての PHP クラス) も生成可能です。\


.. code-block:: bash

    php app/console doctrine:generate:entities AcmeStoreBundle
    php app/console doctrine:generate:entities Acme

.. note::

    Doctrine 自体は、プロパティが ``protected`` なのか ``private`` なのか、といったことや、\
    ゲッターやセッターがあるかどうか、ということは気にしません。\
    ここで作ったゲッターやセッターは、単に実装者が PHP オブジェクトを扱う上で必要であるからです。

データベーステーブル/スキーマの作成
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine の永続化用マッピング情報を持った ``Product`` クラスが使用に耐えうる形でできました。\
ですが、データベースには、まだ、エンティティクラスに対応する ``product`` テーブルがありません。\
幸運にも、Doctrine は、アプリケーション内で既知のエンティティが必要としているテーブルを、自動的にすべて作成することができます。\
次のコマンドを実行してください。

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. tip::

    実際、このコマンドは信じられないくらい強力です。\
    (エンティティのマッピング情報に基づいた)データベースが\ *どうなっているべき*\ なのか、ということと、\
    *今どうなっている*\ のかということを比較して、\
    データベースの\ *更新*\ に必要な SQL を作成します。\
    つまり、\ ``Product`` に metadata とともにプロパティを追加して、このタスクを実行すると、\
    現状の ``product`` テーブルに新しいカラムを追加する "alter table" ステートメントが作成されます。

    さらにこの機能をうまく使うには、\ :doc:`マイグレーション</bundles/DoctrineMigrationsBundle/index>` を通すことでしょう。\
    マイグレーションでは、SQL を作成して、それらをマイグレーションクラスにストアします。\
    このクラスは、プロダクションサーバーで、データベーススキーマのトラッキングとマイグレートを、\
    安全で信頼性をもってシステマチックに行うために使用されます。

これで、指定した metadata に合致するカラムを備えた完全機能な ``product`` がデータベースにできました。

データベースへのオブジェクトの永続化
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

さて、エンティティである ``Product`` が、対応するテーブルである ``product`` にマップされたので、\
データベースへのデータの永続化の用意は整いました。\
永続化は、コントローラ内でとても簡単に実行できます。\
バンドルの ``DefaultController`` に次のようなメソッドを追加してみましょう。

.. code-block:: php
    :linenos:

    // src/Acme/StoreBundle/Controller/DefaultController.php
    use Acme\StoreBundle\Entity\Product;
    use Symfony\Component\HttpFoundation\Response;
    // ...
    
    public function createAction()
    {
        $product = new Product();
        $product->setName('A Foo Bar');
        $product->setPrice('19.99');
        $product->setDescription('Lorem ipsum dolor');

        $em = $this->getDoctrine()->getEntityManager();
        $em->persist($product);
        $em->flush();

        return new Response('Created product id '.$product->getId());
    }

.. note::

    この例を追っている方は、動作確認のために、このアクションを示すルートを作る必要があります。

例を一つ一つみていきましょう。

* **lines 8-11** この部分では、普通の PHP オブジェクトと同様に、\ ``$product`` オブジェクトをインスタンス化して使用しています。

* **line 13** Doctrine の *エンティティマネージャ* オブジェクトを取得しています。\
  このオブジェクトは、データベースへの永続化処理、データベースからのフェッチ処理を扱います。

* **line 14** ``persist()`` メソッドで Doctrine に ``$product`` オブジェクトを "manage" するように伝えています。\
  実際には、データベースへのクエリは(まだ)発生しません。

* **line 15** ``flush()`` メソッドが呼ばれると、"manage" している全てのオブジェクトを見て、\
  データベースに永続化される必要があるのかを判断します。\
  この例では、\ ``$product`` は、まだ永続化されていませんので、\
  エンティティマネージャは、\ ``INSERT`` クエリを実行し、\ ``product`` テーブルに行が作られます。

.. note::

  Doctrine は、manage しているエンティティを全て知っているので、実際には、\
  ``fulsh()`` メソッドが呼ばれたときに、変更点を全て計算し、\
  可能なかぎりクエリが効率的になるように実行します。\
  例えば、100個の ``Product`` オブジェクトを永続化しようと ``flush()`` を呼ぶと、\
  Doctrine は *ただ1つ* のプリペアドステートメントを作成し、それぞれの INSERT で再使用します。\
  これを、*Unit of Work* パターンと呼び、速度と効率性の観点から使用されています。\


オブジェクトの作成でも更新でも、ワークフローは同じです。\
次節では、データベース内にすでにレコードを持っている場合に、\
動的に ``UPDATE`` クエリを発行するという、Doctrine の賢いところを見ていきます。

.. tip::

    Doctrine は、プログラムでテストデータ(フィクスチャデータ)をプロジェクトにロードするライブラリを提供しています。\
    詳細は、\ :doc:`/bundles/DoctrineFixturesBundle/index` を参照してください。


データベースからのオブジェクトのフェッチ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

データベースからオブジェクトをフェッチしてくるのは、もっと簡単です。\
``id`` の値から特定の ``Product`` を表示するルートを設定したとしましょう。\ ::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);
        
        if (!$product) {
            throw $this->createNotFoundException('No product found for id '.$id);
        }

        // do something, like pass the $product object into a template
    }

ある特定の種類のオブジェクトに対するクエリの場合は、"repository(リポジトリ)" を使います。\
リポジトリは、特定のクラスのエンティティのフェッチを補助するためだけの PHP クラスと考えてよいでしょう。\
あるエンティティクラスに対するリポジトリオブジェクトには、次のようにアクセスできます。\ ::

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

.. note::

    ``AcmeStoreBundle:Product`` という文字列は Doctrine 内で共通して使えるショートカットで、\
    クラスへのフルパス(``Acme\StoreBundle\Entity\Product``)を代替します。\
    エンティティが、バンドル内の ``Entity`` という名前空間に存在していれば、このショートカットは有効です。


リポジトリを取得すれば、たくさんの便利なメソッドにアクセスできるようになります。\ ::

    // プライマリーキー(通常は"id")でクエリ
    $product = $repository->find($id);

    // あるカラム値に基づいて find する、動的なメソッド名
    $product = $repository->findOneById($id);
    $product = $repository->findOneByName('foo');

    // 
    // *すべて* の商品を find
    $products = $repository->findAll();

    // 任意のカラム値に基づく、商品群の find
    $products = $repository->findByPrice(19.99);

.. note::

    もちろん、複雑なクエリも扱うことができます。\
    :ref:`book-doctrine-queries` 節を参照してください。　


複数の条件によるオブジェクトのフェッチも、便利な ``findBy`` や ``findOneBy`` メソッドを\
うまく使ってやることにより可能です。 ::

    // name と price の両方にマッチする1つの商品を取得するクエリ
    $product = $repository->findOneBy(array('name' => 'foo', 'price' => 19.99));

    // name にマッチするすべての商品を price 順で取得するクエリ
    $product = $repository->findBy(
        array('name' => 'foo'),
        array('price' => 'ASC')
    );

.. tip::

    ページがレンダリングされるときは、何本のクエリが走ったのか、\
    web debug toolbar の右下で確認することができます。

    .. image:: /images/book/doctrine_web_debug_toolbar.png
       :align: center
       :scale: 50
       :width: 350

    アイコンをクリックすると、profiler が開き、どんなクエリが発行されたのかが表示されます。

オブジェクトのアップデート
~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine でオブジェクトのフェッチができたら、それをアップデートすることは簡単です。\
商品IDとアップデートアクションをマップするようなルートを考えてみましょう。\ :: 

    public function updateAction($id)
    {
        $em = $this->getDoctrine()->getEntityManager();
        $product = $em->getRepository('AcmeStoreBundle:Product')->find($id);

        if (!$product) {
            throw $this->createNotFoundException('No product found for id '.$id);
        }

        $product->setName('New product name!');
        $em->flush();

        return $this->redirect($this->generateUrl('homepage'));
    }

オブジェクトのアップデートは次の3ステップからなります。

1. Doctrine からオブジェクトを取得する
2. オブジェクトに変更を加える
3. エンティティマネージャの ``flush()`` を呼ぶ

``$em->persist($product)`` の呼び出しが必要でないことが分かります。\
このメソッドは、Doctrine に ``$product`` オブジェクトを "manage" もしくは "監視" するように伝えるメソッドだったことを思い出してください。\
この例では、\ ``$product`` オブジェクトを Doctrine からフェッチしており、すでに "manage" されているのです。

オブジェクトの削除
~~~~~~~~~~~~~~~~~~

オブジェクトの削除も同様ですが、エンティティマネージャの ``remove()`` メソッドを呼ぶ必要があります。\ ::

    $em->remove($product);
    $em->flush();

ご期待のとおり、\ ``remove()`` メソッドは、与えられたエンティティをデータベースから削除したい、ということを Doctrine に伝えるものです。\
ただし、\ ``flush()`` メソッドが呼ばれるまでは、実際には削除されません。

.. _`book-doctrine-queries`:

クエリ
------

リポジトリオブジェクトを使えば、特に何もしなくても基本的なクエリであれば実行可能であることはわかりました。\ ::

    $repository->find($id);
    
    $repository->findOneByName('Foo');

もちろん、Doctrine では、より複雑なクエリを Doctrine Query Language (DQL) を使用して書くことも可能です。\
DQL は SQL と似ていますが、テーブル(例: ``product``)の行ではなくて、\
1つ以上のエンティティクラスオブジェクト(例: ``Product``)に対してクエリするということを想定しなければなりません。\

Doctrne でクエリするには、2つの選択肢があります。\
純粋な Doctrine クエリを書くか、Doctrine の Query Buider を使用することです。


DQL でクエリ
~~~~~~~~~~~~

商品を検索する際に、値段として\ ``19.99`` 以上の商品のみを、安い順に返したいとします。\
コントローラ内で、下記のように行います。\ ::

    $em = $this->getDoctrine()->getEntityManager();
    $query = $em->createQuery(
        'SELECT p FROM AcmeStoreBundle:Product p WHERE p.price > :price ORDER BY p.price ASC'
    )->setParameter('price', '19.99');
    
    $products = $query->getResult();

SQL に慣れていれば、DQL は、とても自然に感じるでしょう。\
一番大きな違いは、データベースの行ではなく、オブジェクトの観点からか考える、というところでしょう。\
こうした理由から、\ ``AcmeStoreBundle:Product`` を *from* として、そのエイリアスとして ``p`` を与えているのです。

``getResult()`` メソッドは、結果の配列を返します。\
1つのオブジェクトのみを期待している場合は、\ ``getSingleResult()`` メソッドを使用します。\ ::

    $product = $query->getSingleResult();

.. caution::

    ``getSingleResult()`` メソッドは、結果がない場合、一つより\ *多く*\ の結果が返ってきたときに、\
    それぞれ、\ ``Doctrine\ORM\NoResultException``\ 、\ ``Doctrine\ORM\NonUniqueResultException`` をスローします。\
    もしこのメソッドを使用する場合は(そして、1つより多くの結果を返すようなクエリを実行している場合は)、try-catch ブロックで囲って、\
    ただひとつの結果が返ることを明確にしておかなければなりません。\ ::


        $query = $em->createQuery('SELECT ....')
            ->setMaxResults(1);
        
        try {
            $product = $query->getSingleResult();
        } catch (\Doctrine\Orm\NoResultException $e) {
            $product = null;
        }
        // ...

DQL 構文は驚異的にパワフルで、エンティティ間の JOIN (:ref:`relations<book-doctrine-relations>` で触れます)や、\
group などを楽に行うことができます。\
より詳細な情報は、Doctrine のドキュメント `Doctrine Query Language`_ を参照してください。

.. sidebar:: パラメータのセット

    ``setParameter()`` メソッドに注目してください。\
    Doctrine を使用する際は、先の例のように、\
    外部的な値は常に"プレースホルダ"として設定するのが良いでしょう。
    
    .. code-block:: text

        ... WHERE p.price > :price ...

    プレースホルダ ``price`` に値をセットするには、\ ``setParameter()`` メソッドを呼びます。\ ::

        ->setParameter('price', '19.99')

    値を直に置くのではなくパラメータを使用するのは、SQL インジェクションを防ぐためであり、\
    常にそうすべきです。\
    複数のパラメータがある時は、\ ``setParameters()`` メソッドを使用すれば一度に値をセット出来ます。\ ::

        ->setParameters(array(
            'price' => '19.99',
            'name'  => 'Foo',
        ))

Query Builder の使用
~~~~~~~~~~~~~~~~~~~~

クエリをそのまま書く代わりに、Doctrine の ``QueryBuilder`` を使えば、\
同等のことを、ナイスでオブジェクト指向なインターフェースを使って行うことができます。\
IDE を使っているのであれば、メソッド名の入力時に自動補完の恩恵をうけることができるでしょう。\
コントローラ内でこのように書いていきます。\ ::

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

    $query = $repository->createQueryBuilder('p')
        ->where('p.price > :price')
        ->setParameter('price', '19.99')
        ->orderBy('p.price', 'ASC')
        ->getQuery();
    
    $products = $query->getResult();

``QueryBuilder`` オブジェクトは、クエリを組み立てるのに必要なメソッド全てを含んでいます。\
``getQuery()`` メソッドを呼ぶと、\ ``Query`` オブジェクトを返します。\
前節で素直に書いた場合も、同じ ``Query`` オブジェクトを返しています。

Doctrine Query Builder に関するより詳細は、Doctrine のドキュメント `Query Builder`_ を参照してください。

カスタムリポジトリクラス
~~~~~~~~~~~~~~~~~~~~~~~~

前節では、コントローラ内でより複雑なクエリを作ることに着手しました。\
クエリを分離すること、テストすること、再利用するためには、\
エンティティのカスタムリポジトリクラスを作成して、クエリのロジックをメソッドとして追加するとよいでしょう。

このためには、マッピング定義にリポジトリクラスの名前を追加します。　

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Entity/Product.php
        namespace Acme\StoreBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity(repositoryClass="Acme\StoreBundle\Repository\ProductRepository")
         */
        class Product
        {
            //...
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            repositoryClass: Acme\StoreBundle\Repository\ProductRepository
            # ...

    .. code-block:: xml

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <!-- ... -->
        <doctrine-mapping>

            <entity name="Acme\StoreBundle\Entity\Product"
                    repository-class="Acme\StoreBundle\Repository\ProductRepository">
                    <!-- ... -->
            </entity>
        </doctrine-mapping>

.. ** <- this is for vim hilighting problem, simply ignore this

リポジトリクラスは、以前にゲッターやセッターメソッドを作成したときに使用した\
コマンドと同じコマンドを実行することで作成できます。

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

次に、できた新しい リポジトリクラスに、メソッド ``findAllOrderedByName()`` を追加してみます。\
すべての ``Product`` エンティティに対して、アルファベット順でクエリするメソッドです。


.. code-block:: php

    // src/Acme/StoreBundle/Repository/ProductRepository.php
    namespace Acme\StoreBundle\Repository;

    use Doctrine\ORM\EntityRepository;

    class ProductRepository extends EntityRepository
    {
        public function findAllOrderedByName()
        {
            return $this->getEntityManager()
                ->createQuery('SELECT p FROM AcmeStoreBundle:Product p ORDER BY p.name ASC')
                ->getResult();
        }
    }

.. tip::

    リポジトリ内部からは、\ ``$this->getEntityManager()`` で、エンティティマネージャにアクセスできます。

この新しいメソッドは、リポジトリのデフォルトのファインダーメソッドのように使用できます。:: 

    $em = $this->getDoctrine()->getEntityManager();
    $products = $em->getRepository('AcmeStoreBundle:Product')
                ->findAllOrderedByName();

.. note::

    カスタムリポジトリクラスを使用している場合でも、\
    ``find()`` や ``findAll()`` といったデフォルトのファインダーメソッドへのアクセスは可能です。

.. _`book-doctrine-relations`:

エンティティのリレーション/アソシエーション
-------------------------------------------

このアプリケーションの商品は、全てある1つの「カテゴリ」に属しているとしましょう。\
この場合、\ ``Category`` オブジェクトが必要になってくるのと、\ ``Product`` オブジェクトをその ``Category`` に関連付ける方法が必要になってきます。\
まずは ``Category`` エンティティを作ることから始めましょう。\
どのみち Doctrine を通して永続化しないといけないのは分かっているので、\
Doctrine にクラスを作らせてみましょう。

.. code-block:: bash

    php app/console doctrine:generate:entity --entity="AcmeStoreBundle:Category" --fields="name:string(255)"

このタスクは、エンティティである ``Category`` を作成し、\
``id`` 及び ``name`` フィールドとそれぞれのゲッター、セッター関数を作成するものです。

Metadata をマッピングするリレーション
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``Category`` と ``Product`` エンティティを関連付けるために、\
まずは、\ ``Category`` クラスに ``products`` プロパティを作成することから始めましょう。\ ::

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Entity/Category.php
        // ...
        use Doctrine\Common\Collections\ArrayCollection;
        
        class Category
        {
            // ...
            
            /**
             * @ORM\OneToMany(targetEntity="Product", mappedBy="category")
             */
            protected $products;
    
            public function __construct()
            {
                $this->products = new ArrayCollection();
            }
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Category.orm.yml
        Acme\StoreBundle\Entity\Category:
            type: entity
            # ...
            oneToMany:
                products:
                    targetEntity: Product
                    mappedBy: category
            # don't forget to init the collection in entity __construct() method

まず、\ ``Category`` クラスは複数(many)の ``Product`` オブジェクトと関連するので、\
プロパティ ``products`` 配列を追加し、このプロパティがそれら ``Product`` オブジェクト群を保持するようにします。\
もう一度言っておきますが、これは Doctrine が必要とするわけではありません。\
アプリケーション内で各 ``Category`` が、\ ``Product`` オブジェクトの配列を持つことに意味があるのです。


.. note::

    ``__construct()`` メソッド内のコードは重要です。\
    なぜなら、Doctrine としては、\ ``$products`` プロパティが ``ArrayCollection`` オブジェクトである必要があるからです。\
    このオブジェクトは、ほとんど配列と\ *同様に*\ ふるまいますが、いくつか柔軟性があります。\
    もしあまり気に入らなくても、特に心配いりません。\
    単に ``array`` であるという風に仮定してください。そうすれば問題ありません。

.. tip::

   上記の Decorator の中の targetEntity の値は同じクラスで定義されたエンティティだけでなく、妥当な名前空間における任意のエンティティを参照できます。異なるクラスもしくはバンドルで定義されたエンティティを関連付けるには、targetEntity として完全な名前空間を入力します。

次に、\ ``Product`` クラスですが、これは、ただ1つ(one) の ``Category`` というオブジェクトと関連しています。\
ですので、\ ``Product`` クラスに ``$category`` プロパティを追加したくなりますよね。\ ::

.. configuration-block::

    .. code-block:: php-annotations


        // src/Acme/StoreBundle/Entity/Product.php
        // ...
    
        class Product
        {
            // ...
        
            /**
             * @ORM\ManyToOne(targetEntity="Category", inversedBy="products")
             * @ORM\JoinColumn(name="category_id", referencedColumnName="id")
             */
            protected $category;
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            # ...
            manyToOne:
                category:
                    targetEntity: Category
                    inversedBy: products
                    joinColumn:
                        name: category_id
                        referencedColumnName: id

さて、これで ``Category`` と ``Product`` クラスの両方に新しいプロパティが追加されましたので、\
Doctrine に足りないゲッターとセッターを作ってもらうようにお願いしましょう。

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

Doctrine の metadata のことは、一瞬忘れてみてください。\
現在、二つのクラス ``Category`` と ``Product`` が、普通の one-to-many リレーションを持っています。\
``Category`` クラスが、\ ``Product`` オブジェクトの配列を持ち、\ ``Product`` が1つの ``Category`` オブジェクトを保持することができます。\
言いたいのは、これは、自分の要件にうまく合うようにクラスを作成した、ということです。\
データベースに永続化する、ということは、常に二番手にくる話です。


では、\ ``Product`` クラスの ``$category`` プロパティの metadata を見てみましょう。\
ここで Doctrine に伝えている情報というのは、\
関連付けられるクラスは ``Category`` で、そのレコードの ``id`` を ``product`` テーブル上の ``category_id`` レコードにストアしろ、ということです。\
つまり、関連付けられる ``Cateogry`` オブジェクトそのものは ``$category`` プロパティにストアされるのですが、\
その裏では、Doctrine がこのリレーションを、\ ``product`` テーブルの ``category_id`` カラム上に、カテゴリのIDをストアすることで永続化していると言えます。


.. image:: /images/book/doctrine_image_2.png
   :align: center

``Category`` オブジェクトの ``$products`` プロパティの metadata はこれよりは重要ではなく、\
リレーションがどのようにマップされているのかを解決するためには、\ ``Product.category`` プロパティを見ろ、と言っているだけです。

続きを見ていく前に、Doctrine に ``category`` テーブル、\ ``product.category_id`` カラム、そして外部キーを追加させるのを忘れないで下さい。

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. note::

    このタスクは、開発時においてのみしか実行するべきではありません。\
    プロダクション環境のデータベースをより堅牢にそしてシステマチックに更新する際は、\ :doc:`Doctrine migrations</bundles/DoctrineMigrationsBundle/index>` を参照してください。

関連するエンティティの保存
~~~~~~~~~~~~~~~~~~~~~~~~~~

では、動いているところを見ていきましょう。コントローラが次のようになっているとしましょう。\ ::

    // ...
    use Acme\StoreBundle\Entity\Category;
    use Acme\StoreBundle\Entity\Product;
    use Symfony\Component\HttpFoundation\Response;
    // ...

    class DefaultController extends Controller
    {
        public function createProductAction()
        {
            $category = new Category();
            $category->setName('Main Products');
            
            $product = new Product();
            $product->setName('Foo');
            $product->setPrice(19.99);
            $product->setDescription('Lorem ipsum dolor');
            // この商品をカテゴリに関連付ける
            $product->setCategory($category);
            
            $em = $this->getDoctrine()->getEntityManager();
            $em->persist($category);
            $em->persist($product);
            $em->flush();
            
            return new Response(
                'Created product id: '.$product->getId().' and category id: '.$category->getId()
            );
        }
    }

これで、\ ``category`` と ``product`` テーブルの両方に、1行づつ新しい行が追加されます。\
新しくできた商品の ``product.category_id`` カラム には、新しくできたカテゴリの ``id`` がセットされます。\
Doctrine がこのリレーションの永続化を行ってくれるのです。

関連するオブジェクトのフェッチ
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

関連するオブジェクトをフェッチする流れは、今までと同じです。\
まずは、\ ``$product`` オブジェクトをフェッチし、関連する ``Category`` にアクセスします。\ ::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        $categoryName = $product->getCategory()->getName();
        
        // ...
    }

この例では、商品の ``id`` に基づいた ``Product`` オブジェクトへのクエリが一つ目のクエリです。\
ここでは、商品データ\ *のみ*\ へのクエリと、結果データを用いての ``$product`` オブジェクトへのハイドレートが行われます。\
その後、\ ``$product->getCategory()->getName()`` 呼び出しが行われると、\
Doctrine が無言で二つ目のクエリを発行します。\
現在の ``Product`` に関連付けられた ``Category`` の取得です。\
そして\ ``$category`` オブジェクトを用意し、返します。


.. image:: /images/book/doctrine_image_3.png
   :align: center

重要なのは、商品に関連したカテゴリに簡単にアクセスできたということと、\
そのカテゴリのデータは、これを問い合わせた時まで取得されない(「遅延読み込み」) ということです。

逆方向からのクエリも可能です。\ ::

    public function showProductAction($id)
    {
        $category = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Category')
            ->find($id);

        $products = $category->getProducts();
    
        // ...
    }

この場合にしても、同様のことが起こります。\
まず、\ ``Category`` オブジェクトへのクエリを行い、\
その後、Doctrine が関連する ``Product`` オブジェクトを取得するクエリを行います。\
ただし、この ``Product`` オブジェクトを取得するのは、そう頼んだ時(``->getProducts()``)だけです。\
変数 ``$products`` は、与えられた ``Category`` の ``category_id`` 値を通して関連している、すべての ``Product`` オブジェクト配列です。

.. sidebar:: リレーションと proxy クラス

    この「遅延読み込み」というのは、Doctrine が、必要な場合に、\
    "proxy" オブジェクトをそのオブジェクトの場所に返していることで、可能になっています。\
    上記の例を見てみましょう。\ ::
    
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        $category = $product->getCategory();

        // "Proxies\AcmeStoreBundleEntityCategoryProxy" が出力される
        echo get_class($category);

    この proxy オブジェクトは本物の ``Category`` オブジェクトを継承したもので、見た目も振る舞いもそのままです。\
    違いは、proxy オブジェクトの場合だと、\ ``Category`` データが必要になるまで(``$cateogry->getName()`` するまで)、\
    Doctrine がクエリするのを遅らせることができるという点です。

    proxy クラスは Doctrine によって生成され、キャッシュディレクトリにストアされます。\
    ``$category`` が実は proxy オブジェクトだということに気づくことは無いとは思いますが、\
    心に留めておくべき重要な点です。

    次節でみていきますが、(*join* を通じて) 商品とカテゴリのデータを一度に取得する場合は、\
    遅延読み込みの必要がないので、Doctrine は\ *本物の* ``Category`` オブジェクトを返します。

関連するレコードの JOIN
~~~~~~~~~~~~~~~~~~~~~~~

上記の例では2つのクエリが作成されました。\
一つは元のオブジェクト(``Cateogry``)に対するもの、\
もうひとつは、関連したオブジェクト(群)(``Product``) です。\


.. tip::

    リクエストに対して生じたクエリはすべて、web debug toolbar を通じて確認できます。

もちろん、両方のオブジェクトにアクセスすることが前もって分かっているときは、\
元のクエリに join することによって2つ目のクエリを避けることができます。\
``ProductRepository`` クラスに次のようなメソッドを追加します。\ ::

    // src/Acme/StoreBundle/Repository/ProductRepository.php
    
    public function findOneByIdJoinedToCategory($id)
    {
        $query = $this->getEntityManager()
            ->createQuery('
                SELECT p, c FROM AcmeStoreBundle:Product p
                JOIN p.category c
                WHERE p.id = :id'
            )->setParameter('id', $id);
        
        try {
            return $query->getSingleResult();
        } catch (\Doctrine\ORM\NoResultException $e) {
            return null;
        }
    }

これでコントローラ内からこのメソッドを使うことで、\ ``Product`` オブジェクトとそれに関連した ``Category`` へのクエリを、\
一度のクエリで行うことができるようになりました。\ ::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->findOneByIdJoinedToCategory($id);

        $category = $product->getCategory();
    
        // ...
    }    

アソシエーションの詳細情報
~~~~~~~~~~~~~~~~~~~~~~~~~~

この節では、一般的なエンティティリレーションの一つである、one-to-many を紹介してきました。\
より高度な詳細情報と、その他のリレーション(``one-to-one`` や ``many-to-many``) の使い方の例は、\
Doctrine の `Association Mapping Documentation`_ を参照してください。


.. note::

    アノテーションを使用している場合は、全てのアノテーションの先頭に ``ORM\`` を付加してください(例： ``ORM\OneToMany``)。\
    これは Doctrine のドキュメントでは反映されていません。\
    また、\ ``use Doctrine\ORM\Mapping as ORM;`` ステイトメントを行う必要もあります。\
    これは、\ ``ORM`` アノテーションプリフィックスを\ *インポート*\ するものです。


設定
----

Doctrine は、おそらくそのほとんどは心配することのないようなオプションですが、\
かなりの範囲で設定が可能となっています。\
Doctrine の設定に関してより知りたい場合は、Doctrine ドキュメントの
:doc:`reference manual</reference/configuration/doctrine>` 節を参照してください。


Lifecycle Callback
------------------

エンティティが INSERT や、UPDATE、DELETE される直前、もしくは直後に、\
何かアクションが必要なこともあるでしょう。\
これらのアクションは、"lifecycle" callback と呼ばれ、\
エンティティのライフサイクル(例えばエンティティがINSERT や、UPDATE、DELETEされるなど)それぞれの間で実行される必要のあるコールバックメソッドということです。

metadata としてアノテーションを使用している場合は、まず、lifecycle callback を有効にしてください。\
YAML や XML を使用している場合は必要ありません。

.. code-block:: php-annotations

    /**
     * @ORM\Entity()
     * @ORM\HasLifecycleCallbacks()
     */
    class Product
    {
        // ...
    }

.. ** <- this is for vim hilighting problem, simply ignore this

これで、すべての全ての有効なライフサイクルイベントにおいて、\
Doctrine にメソッドを実行するように伝えることができるようになりました。\
あるエンティティが初めて永続化(INSERT)される際に、\ ``created`` という日付のカラムへ現在の日付を入れたいとしましょう。

.. configuration-block::

    .. code-block:: php-annotations

        /**
         * @ORM\prePersist
         */
        public function setCreatedValue()
        {
            $this->created = new \DateTime();
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            # ...
            lifecycleCallbacks:
                prePersist: [ setCreatedValue ]

    .. code-block:: xml

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <!-- ... -->
        <doctrine-mapping>

            <entity name="Acme\StoreBundle\Entity\Product">
                    <!-- ... -->
                    <lifecycle-callbacks>
                        <lifecycle-callback type="prePersist" method="setCreatedValue" />
                    </lifecycle-callbacks>
            </entity>
        </doctrine-mapping>

.. ** <- this is for vim hilighting problem, simply ignore this

.. note::

    上記の例では、\ ``created`` プロパティの作成とマッピングは終わっているものとします(ここでは書いていません)。

これで、エンティティが初めて永続化される直前に、\
Doctrine は自動的にこのメソッドを呼ぶようになり、\ ``created`` フィールドは現在の日付に設定されるようになります。

これは、次のような他のライフサイクルイベントでも同じことが行われます。

* ``preRemove``
* ``postRemove``
* ``prePersist``
* ``postPersist``
* ``preUpdate``
* ``postUpdate``
* ``postLoad``
* ``loadClassMetadata``

これらのライフサイクルの意味や、lifecycle callback 一般については、Doctrine の `Lifecycle Events documentation`_ を見てください。


.. sidebar:: Lifecycle Callback とイベントリスナ

    ``setCreatedValue()`` に引数がないことに注意してください。\
    これはその他のライフサイクルにも言えることですが、意図的にこうなっています。\
    lifecycle callback は、エンティティ内部で変換される(created/updated フィールドや slug 値を生成するなど)ようなシンプルなメソッドであるべきです。
    
    もしもっと重い処理、例えばログを取るとかメールを送るといったような処理が必要な場合は、\
    別のクラスをイベントリスナやサブスクライバとして登録して、\
    それに必要なリソースを与えるべきです。\
    詳細は :doc:`/cookbook/doctrine/event_listeners_subscribers` を参照してください。

Doctrine のエクステンション: Timestampable、Sluggable など
-----------------------------------------------------------

Doctrine は非常に柔軟性に富んでおり、たくさんのサードパーティ製エクステンションが使用可能になっており、\
エンティティに対して度々、そして一般的に起こりうるタスクを簡単にこなしてくれます。\
*Sluggable*\ 、\ *Timestampable*\ 、\ *Loggable*\ 、\ *Translatable* や *Tree* などがあります。

これらエクステンションの探し方やその使い方についてはクックブックの
:doc:`「共通の Doctrine エクステンションのドキュメント」 </cookbook/doctrine/common_extensions>` を参照してください。

.. _book-doctrine-field-types:

Doctrine フィールドタイプリファレンス
-------------------------------------

Doctrine は、たくさんのフィールドタイプが使用可能です。\
それぞれ、PHP のデータタイプが、データベースのカラムタイプ(どんなデータベースでも)にマップされています。\
Doctrine では、下記のフィールドタイプがサポートされています。


* **文字**

  * ``string`` (短めの文字列)
  * ``text`` (長めの文字列)

* **数**

  * ``integer``
  * ``smallint``
  * ``bigint``
  * ``decimal``
  * ``float``

* **日付と時刻** (PHP 上では `DateTime`_ オブジェクトを使用します)

  * ``date``
  * ``time``
  * ``datetime``

* **その他**

  * ``boolean``
  * ``object`` (シリアライズされ ``CLOB`` にストアされます)
  * ``array`` (シリアライズされ ``CLOB`` にストアされます)

詳細は Doctrine の `Mapping Types documentation`_ を参照してください。

フィールドオプション
~~~~~~~~~~~~~~~~~~~~

各フィールドには、それぞれ適用できるオプション群があります。\
使用可能なオプションには、\ ``type`` (デフォルトは ``string``)、\ ``name``\ 、\ ``length``\ 、\ ``unique`` や ``nullable`` があります。\
いくつかアノテーションの例を見てみましょう。


.. code-block:: php-annotations

    /**
     * 長さ 255 で null 不可の string
     * ("type"、"length"、 *nullable* オプションはデフォルト値が反映されています)
     * 
     * @ORM\Column()
     */
    protected $name;

    /**
     * 長さ 150 で "email_address" カラムに永続される string
     * unique index もつきます
     *
     * @ORM\Column(name="email_address", unique="true", length="150")
     */
    protected $email;

.. ** <- this is for vim hilighting problem, simply ignore this

.. note::

    ここには挙げていませんが、もういくつかオプションがあります。\
    詳細は Doctrine の `Property Mapping documentation`_ を参照してください。

.. index::
   single: Doctrine; ORM Console Commands
   single: CLI; Doctrine ORM

コンソールコマンド
------------------

Doctrine2 ORM を統合していることで、たくさんのコンソールコマンド(名前空間は ``doctrine``)が付いてきます。\
コマンドのリストは、引数なしでコンソールを実行します。

.. code-block:: bash

    php app/console

使用可能なコマンドのリストが表示されます。\
そのうちの多くに、\ ``doctrine:`` というプリフィックスが付いています。\
これらのコマンド(もしくは、Symfony コマンド) の詳細が知りたい場合は、\ ``help`` コマンドを実行します。\
``doctrine:database:create`` の詳細が知りたい場合は、次のように実行します。 

.. code-block:: bash

    php app/console help doctrine:database:create

注目すべき、もしくは興味深いタスクを挙げてみます。

* ``doctrine:ensure-production-settings`` - 現在の環境がプロダクション環境に相応しいように設定されているかをチェックします。\
  これは、常に ``prod`` 環境で実行されることを想定しています。
  
  .. code-block:: bash
  
    php app/console doctrine:ensure-production-settings --env=prod

* ``doctrine:mapping:import`` - 既存のデータベースを調査して、マッピング情報を作成します。\
  詳細は、\ :doc:`/cookbook/doctrine/reverse_engineering` を参照のこと。


* ``doctrine:mapping:info`` - Doctrine が把握しているエンティティを教えてくれます。\
   また、マッピングに基本的なエラーがあるかどうかも示します。

* ``doctrine:query:dql`` と ``doctrine:query:sql`` - DQL もしくは SQL をコマンドラインから直に実行できます。

.. note::

   フィクスチャデータをデータベースにロードするには、\ ``DoctrineFixturesBundle`` のインストールが必要です。\
   詳細は、":doc:`/bundles/DoctrineFixturesBundle/index`" を参照してください。

まとめ
------

.. todo brushup

Doctrine を使用することで、オブジェクトとそれを便利に使うという点に集中でき、\
データベースへの永続化は一つ後の心配事とすることができます。\
これは、データをあらゆる PHP オブジェクトにもつことを Doctrine が許しているからであり、\
Doctrine が、マッピング情報である metadata を通して、オブジェクトのデータを特定のデータベーステーブルにマップしているためです。

Doctrine はシンプルなコンセプトを中心にしてはいるのですが、信じられないくらい強力です。\
複雑なクエリを作成したり、\
イベントをサブスクライブすることで、永続化ライフサイクルを通じて異なるアクションを展開することが可能になっています。

Doctrineについてのより詳細な情報は、\ :doc:`cookbook</cookbook/index>` の *Doctrine* を参照してください。\
次のような記事があります。

* :doc:`/bundles/DoctrineFixturesBundle/index`
* :doc:`/cookbook/doctrine/common_extensions`

.. _`Doctrine`: http://www.doctrine-project.org/
.. _`MongoDB`: http://www.mongodb.org/
.. _`Basic Mapping Documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html
.. _`Query Builder`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/query-builder.html
.. _`Doctrine Query Language`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/dql-doctrine-query-language.html
.. _`Association Mapping Documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/association-mapping.html
.. _`DateTime`: http://php.net/manual/en/class.datetime.php
.. _`Mapping Types Documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html#doctrine-mapping-types
.. _`Property Mapping documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html#property-mapping
.. _`Lifecycle Events documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/events.html#lifecycle-events
.. _`Reserved SQL keywords documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/basic-mapping.html#quoting-reserved-words


.. 2011/08/13 gilbite 7d4960d
.. 2011/08/27 hidenorigoto 89d5fd5d7caac1dac690a62f1551dd0b5b3d6b8a
.. 2012/01/10 masakielastic afa2a23c80a182379ac72708399832ae7886158b
