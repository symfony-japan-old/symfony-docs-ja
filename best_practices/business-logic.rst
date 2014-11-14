.. note::

    * 対象バージョン：2.3以降
    * 翻訳更新日：2014/11/05

アプリケーションのビジネスロジックを整理する
==============================================

コンピュータのソフトウェアの世界では、データがどのように作られ、表示され、保存され、変更されるかを決める
現実世界のビジネスルールを実装したプログラムを  **ビジネスロジック** あるいはドメインロジックと呼びます。
（ `完全な定義`_ を読む）

Symfony のアプリケーションでは、ビジネスロジックは、フレームワーク（例えばルーティングやコントローラの
ような）に依存せず、自由に書くことができます。
サービスとして使われる、ドメインのクラス・ Doctrine のエンティティ・通常の PHP のクラスは、ビジネスロジック
の一例です。

ほとんどのプロジェクトにおいて、開発者は全てを ``AppBundle`` に置くべきです。AppBundleの中では、ビジネス
ロジックを整理するための、どんなディレクトリでも作ることができます。

.. code-block:: text

    symfoy2-project/
    ├─ app/
    ├─ src/
    │  └─ AppBundle/
    │     └─ Utils/
    │        └─ MyClass.php
    ├─ vendor/
    └─ web/

クラスをバンドルの外に置く
--------------------------------------

しかし、ビジネスロジックをバンドルの中に入れておく技術的な理由は何もありません。 ``src`` ディレクトリの
下に好きな名前空間を定義して、クラスをそこに置くこともできます。

.. code-block:: text

    symfoy2-project/
    ├─ app/
    ├─ src/
    │  ├─ Acme/
    │  │   └─ Utils/
    │  │      └─ MyClass.php
    │  └─ AppBundle/
    ├─ vendor/
    └─ web/

.. tip::

    ``AppBundle`` を使うことをお勧めするのは簡単さのためです。バンドルの中に必要なものと
    バンドルの外でも問題ないものがよく理解できている場合は、クラスを ``AppBundle`` の外に
    置いても構いません。

サービス：名付けと形式
---------------------------

ブログアプリケーションに、 "Hello World" のような投稿タイトルを "hello-world" のようなスラグに変換する
ユーティリティが必要だとします。スラグは、投稿のURLの一部として使われます。

新しい ``Slugger`` クラスを ``src/AppBundle/Utils/`` ディレクトリの配下に作り、下記のような ``slugify()``
メソッドを実装してみましょう。

.. code-block:: php

    // src/AppBundle/Utils/Slugger.php
    namespace AppBundle\Utils;

    class Slugger
    {
        public function slugify($string)
        {
            return preg_replace(
                '/[^a-z0-9]/', '-', strtolower(trim(strip_tags($string)))
            );
        }
    }

次に、このクラスのための新しいサービスを定義します。

.. code-block:: yaml

    # app/config/services.yml
    services:
        # サービス名は短くしましょう
        slugger:
            class: AppBundle\Utils\Slugger

伝統的に、サービスの名前は、衝突を避けるためにクラス名とクラスの場所を組み合わせたものでした。
そうすると、このサービスは ``app.utils.slugger`` と呼ばれる *はず* です。しかし、短い名前を使うことで、
コードの読みやすさと使いやすさは向上するでしょう。

.. best-practice::

    アプリケーションのサービス名は可能な限り短くしましょう。一単語になるのが理想的です。

これで ``AdminController`` のようなどんなコントローラーからでも slugger を利用できるようになりました。

.. code-block:: php

    public function createAction(Request $request)
    {
        // ...

        if ($form->isSubmitted() && $form->isValid()) {
            $slug = $this->get('slugger')->slugify($post->getTitle()));
            $post->setSlug($slug);

            // ...
        }
    }

サービス定義：YAML形式
--------------------------

前のセクションでは、 YAML がサービスを定義するのに使われていました。

.. best-practice::

    サービスを定義するときは YAML 形式を使いましょう。

これには異論があるでしょうが、経験上、開発者の間では YAML と XML が半々で使われており、ほんの少し YAML
のほうが好まれています。
どちらの形式も機能は同じなので、どこまでも個人の好みの問題です。

新人にもわかりやすく、シンプルな YAML をお勧めしますが、好きな形式を使って構いません。

サービス定義：クラス名をパラメータにしない
-------------------------------------------

前の例で、サービスを定義するとき、クラス名をパラメータとして定義していないことにお気付きかもしれません。

.. code-block:: yaml

    # app/config/services.yml

    # クラス名をパラメータにしてサービスを定義
    parameters:
        slugger.class: AppBundle\Utils\Slugger

    services:
        slugger:
            class: "%slugger.class%"

この使い方は煩雑で、アプリケーションのサービスには全く必要ありません。

.. best-practice::

    アプリケーションのサービスクラス名をパラメータとして定義するのは止めましょう。    

この使い方はサードパーティのバンドルから誤って取り入れられたものです。 Symfony がサービスコンテナ機能を
実装したとき、開発者の中にはこのテクニックによってサービスを上書きできるようにした人もいました。
しかし、クラス名を変更しただけでサービスを上書きするのは非常に稀なユースケースです。というのも、大抵の場合、
新しいサービスには、上書きされるサービスとは違うコンストラクタ引数があるからです。

永続化レイヤーを利用する
-------------------------

Symfony は、 各 HTTP リクエストに対する HTTP レスポンスを作ることだけを担当する HTTP のフレームワークです。
そのため、 Symfony は永続化レイヤー（データベースや外部API）にアクセスする方法を提供していません。開発者は
好きなライブラリやデータ保存方法を選ぶことができます。

実際には、 Symfony アプリケーションの多くは `Doctrine`_ に依存しており、エンティティやレポジトリを
使ってモデルを定義しています。
ビジネスロジックと同じく、 Doctrine のエンティティも ``AppBundle`` に配置すると良いでしょう。

一例として、サンプルのブログアプリケーションで定義した3つのエンティティがあります。

.. code-block:: text

    symfony2-project/
    ├─ ...
    └─ src/
       └─ AppBundle/
          └─ Entity/
             ├─ Comment.php
             ├─ Post.php
             └─ User.php

.. tip::

    もちろん、独自の名前空間で ``src/`` の直下にエンティティを置くこともできます。

Doctrine のマッピング
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine のエンティティは、データベースに保存することができるプレーンな PHP オブジェクトです。
Doctrine は、クラスに対して定義されたマッピングメタデータによってエンティティを扱います。マッピングメタデータ
を定義するには YAML, XML, PHP, アノテーション形式が利用できます。

.. best-practice::

    Doctrine エンティティのマッピングにはアノテーションを使いましょう。    

アノテーションは、マッピングを定義したり探したりするのに、現在のところ最も便利で素早く使える形式です。

.. code-block:: php

    namespace AppBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Doctrine\Common\Collections\ArrayCollection;

    /**
     * @ORM\Entity
     */
    class Post
    {
        const NUM_ITEMS = 10;

        /**
         * @ORM\Id
         * @ORM\GeneratedValue
         * @ORM\Column(type="integer")
         */
        private $id;

        /**
         * @ORM\Column(type="string")
         */
        private $title;

        /**
         * @ORM\Column(type="string")
         */
        private $slug;

        /**
         * @ORM\Column(type="text")
         */
        private $content;

        /**
         * @ORM\Column(type="string")
         */
        private $authorEmail;

        /**
         * @ORM\Column(type="datetime")
         */
        private $publishedAt;

        /**
         * @ORM\OneToMany(
         *      targetEntity="Comment",
         *      mappedBy="post",
         *      orphanRemoval=true
         * )
         * @ORM\OrderBy({"publishedAt" = "ASC"})
         */
        private $comments;

        public function __construct()
        {
            $this->publishedAt = new \DateTime();
            $this->comments = new ArrayCollection();
        }

        // getters and setters ...
    }

全てのメタデータ定義形式に同じ機能があり、何度も書いたようにどの形式を使うかは開発者の自由です。

データフィクスチャー
~~~~~~~~~~~~~~~~~~~~~~~

Symfony にはデフォルトではデータフィクスチャー機能は存在しないため、フィクスチャーを扱うためには下記のコマンドを
実行して DoctrineFixturesBundle をインストールする必要があります。

.. code-block:: bash

    $ composer require "doctrine/doctrine-fixtures-bundle"

バンドルを ``AppKernel.php`` で有効化します。その際、 ``dev`` 環境と ``test`` 環境だけにしてください。

.. code-block:: php

    use Symfony\Component\HttpKernel\Kernel;

    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
            );

            if (in_array($this->getEnvironment(), array('dev', 'test'))) {
                // ...
                $bundles[] = new Doctrine\Bundle\FixturesBundle\DoctrineFixturesBundle(),
            }

            return $bundles;
        }

        // ...
    }


単純さを保つために、 `フィクスチャークラス`_ は *1つ* だけ作ることをおすすめします。クラスが大きくなりすぎる
場合はもっとたくさんのフィクスチャークラスを作っても構いません。

少なくとも1つのフィクスチャークラスがあり、データベースへのログイン情報が正しく設定されているのを確認したら、
下記のコマンドを実行するとフィクスチャーを読み込ませることができます。

.. code-block:: bash

    $ php app/console doctrine:fixtures:load

    Careful, database will be purged. Do you want to continue Y/N ? Y
      > purging database
      > loading AppBundle\DataFixtures\ORM\LoadFixtures


コーディング規約
------------------

Symfonh のソースコードは、 PHP コミュニティで定められた `PSR-1`_ と `PRS-2`_ のコーディング規約に
従っています。詳しくは `Symfonyのコーディング規約`_ を読んでください。
または、`PHP-CS-Fixer`_ コマンドを使うこともできます。PHP-CS-Fixerはコードベース全体のコーディング規約を
ほんのの数秒で修正することができるコマンドラインツールです。

.. _`完全な定義`: http://en.wikipedia.org/wiki/Business_logic
.. _`Doctrine`: http://www.doctrine-project.org/
.. _`フィクスチャークラス`: http://symfony.com/doc/master/bundles/DoctrineFixturesBundle/index.html#writing-simple-fixtures
.. _`PSR-1`: http://www.php-fig.org/psr/psr-1/
.. _`PSR-2`: http://www.php-fig.org/psr/psr-2/
.. _`Symfonyのコーディング規約`: http://symfony.com/doc/current/contributing/code/standards.html
.. _`PHP-CS-Fixer`: https://github.com/fabpot/PHP-CS-Fixer

.. 2014/11/05 77web 0f1cf411a0bb630205ce4ac2c5e75d237384f8dc
