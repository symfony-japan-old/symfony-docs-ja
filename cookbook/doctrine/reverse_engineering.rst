.. index::
   single: Doctrine; Generating entities from existing database

.. note::

    * 対象バージョン：2.3+
    * 翻訳更新日：2014/02/19


既にあるデータベースからエンティティを生成する方法
==================================================

データベースを使用する新しいプロジェクトで、開発を始める際には、一般的に２つのシチュエーションが考えられます。ほとんどの場合、データベースのモデルは設計され、スクラッチで作成されます。しかし、毎回そうとは限りません。既にあるデータベースを使用しており、モデルを変更できない場合から始めるときもあります。幸運なことに、 Doctrine の多くの付属のツールが既存のデータベースからモデルクラスを生成するのを助けてくれます。

.. note::

    `Doctrine tools documentation`_ のドキュメントにあるように、リバースエンジニアリングは、プロジェクトを始める際に一度だけ行うものです。 Doctrine は、フィールド、インデックス、外部キー制約に基づく必須なマッピング情報の全てを変換することはできません。 Doctrine は逆のアソシエーション、継承のタイプ、エンティティの様々な動作まで知ることができません。エンティティの様々な動作とは、アソシエーション上のプライマリーキーとしての外部キーを持つことだったり、スケードやライフサイクルイベントのようなセマンティックなオペレーションです。

このチュートリアルでは ``blog_post`` と ``blog_comment`` という２つのテーブルを持つシンプルなブログアプリケーションを使用することを想定しています。コメントのレコードは、外部キー制約を使用して投稿のレコードに結びついています。

.. code-block:: sql

    CREATE TABLE `blog_post` (
      `id` bigint(20) NOT NULL AUTO_INCREMENT,
      `title` varchar(100) COLLATE utf8_unicode_ci NOT NULL,
      `content` longtext COLLATE utf8_unicode_ci NOT NULL,
      `created_at` datetime NOT NULL,
      PRIMARY KEY (`id`)
    ) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

    CREATE TABLE `blog_comment` (
      `id` bigint(20) NOT NULL AUTO_INCREMENT,
      `post_id` bigint(20) NOT NULL,
      `author` varchar(20) COLLATE utf8_unicode_ci NOT NULL,
      `content` longtext COLLATE utf8_unicode_ci NOT NULL,
      `created_at` datetime NOT NULL,
      PRIMARY KEY (`id`),
      KEY `blog_comment_post_id_idx` (`post_id`),
      CONSTRAINT `blog_post_id` FOREIGN KEY (`post_id`) REFERENCES `blog_post` (`id`) ON DELETE CASCADE
    ) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

レシピに入る前に、 ``app/config/parameters.yml`` ファイルなどのデータベースコンフィギュレーションファイルのデータベース接続のパラメータが正しくセットアップされているか調べてください。そして、エンティティクラスを置くことになるバンドルを初期化しておいてください。このチュートリアルでは、 ``src/Acme/BlogBundle`` フォルダに ``AcmeBlogBundle`` があることを想定しています。

既存のデータベースからエンティティクラスを作成するための最初のステップは、 Doctrine にデータベースの内部を調べさせて、一致するメタデータのファイルを生成することです。メタデータのファイルは、テーブルのフィールドに基づいて生成されたエンティティクラスを記述しています。

.. code-block:: bash

    $ php app/console doctrine:mapping:import --force AcmeBlogBundle xml

このコマンドラインツールは、 Doctrine にデータベースの内部を調べさせて、 あなたのバンドルのフォルダである ``src/Acme/BlogBundle/Resources/config/doctrine`` 以下に XML のメタデータファイルを生成します。
実行すると2つのファイルが生成されます。 ``BlogPost.orm.xml`` と ``BlogComment.orm.xml`` です。

.. tip::

    上記のコマンドで、最後の引数を `yml` に変更すれば、メタデータのクラスを YAML フォーマットで生成することもできます。

生成された ``BlogPost.orm.xml`` メタデータファイルは以下のようになります。
:

.. code-block:: xml

    <?xml version="1.0" encoding="utf-8"?>
    <doctrine-mapping>
      <entity name="Acme\BlogBundle\Entity\BlogPost" table="blog_post">
        <change-tracking-policy>DEFERRED_IMPLICIT</change-tracking-policy>
        <id name="id" type="bigint" column="id">
          <generator strategy="IDENTITY"/>
        </id>
        <field name="title" type="string" column="title" length="100" nullable="false"/>
        <field name="content" type="text" column="content" nullable="false"/>
        <field name="createdAt" type="datetime" column="created_at" nullable="false"/>
        <lifecycle-callbacks/>
      </entity>
    </doctrine-mapping>

メタデータファイルが生成されれば、 Doctrine にスキーマをインポートし、関連するエンティティクラスを作成するように尋ねることができます。次のコマンドを実行してください。

.. code-block:: bash

    $ php app/console doctrine:mapping:convert annotation ./src
    $ php app/console doctrine:generate:entities AcmeBlogBundle

最初のコマンドは、アノテーションマッピングをしたエンティティクラスを生成します。マッピングとしてアノテーションではなく YAML や XML を使いたい場合は二つ目のコマンドだけを実行してください。

.. tip::

   アノテーションマッピングを使用する場合、上の二つのコマンドの実行後に XML (または YAML)のマッピングファイルは削除して構いません。

新しく作成された ``BlogComment`` エンティティクラスは以下のようになります。

.. code-block:: php

    <?php

    // src/Acme/BlogBundle/Entity/BlogComment.php
    namespace Acme\BlogBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;

    /**
     * Acme\BlogBundle\Entity\BlogComment
     *
     * @ORM\Table(name="blog_comment")
     * @ORM\Entity
     */
    class BlogComment
    {
        /**
         * @var int $id
         *
         * @ORM\Column(name="id", type="int")
         * @ORM\Id
         * @ORM\GeneratedValue(strategy="IDENTITY")
         */
        private $id;

        /**
         * @var string $author
         *
         * @ORM\Column(name="author", type="string", length=100, nullable=false)
         */
        private $author;

        /**
         * @var text $content
         *
         * @ORM\Column(name="content", type="text", nullable=false)
         */
        private $content;

        /**
         * @var datetime $createdAt
         *
         * @ORM\Column(name="created_at", type="datetime", nullable=false)
         */
        private $createdAt;

        /**
         * @var BlogPost
         *
         * @ORM\ManyToOne(targetEntity="BlogPost")
         * @ORM\JoinColumn(name="post_id", referencedColumnName="id")
         */
        private $post;
    }

このように、 Doctrine は全てのテーブルのフィールドをプライベートでアノテーションされたクラスのプロパティに変換します。最も注目すべきことは、外部キー制約に基づいた ``BlogPost`` エンティティクラスとのリレーションも検知することです。そして、 ``BlogComment`` エンティティクラスにプライベートな ``$post`` プロパティが ``BlogPost`` エンティティをマップされます。

.. note::

   エンティティクラスに一対多のリレーションを追加したい場合は、エンティティ、 XML 又は YAML ファイルに手動で追加する必要があります。
   必要なエンティティに ``inversedBy`` と ``mappedBy`` を含む一対多のセクションを追加してください。

.. _`Doctrine tools documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/latest/reference/tools.html#reverse-engineering

.. 2011/11/01 ganchiku d739c578e765de86a8ad54d6ce9cd32b8a098a1f
.. 2014/02/19 77web 4299eb11ac3cbd0dbf52e337fdb5b375ca08e843