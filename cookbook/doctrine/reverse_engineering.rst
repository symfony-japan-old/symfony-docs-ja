.. index::
   single: Doctrine; Generating entities from existing database

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
      PRIMARY KEY (`id`),
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

    php app/console doctrine:mapping:convert xml ./src/Acme/BlogBundle/Resources/config/doctrine/metadata/orm --from-database --force

このコマンドラインツールは、 Doctrine にデータベースの内部を調べさせて、 あなたのバンドルのフォルダである ``src/Acme/BlogBundle/Resources/config/doctrine/metadata/orm`` 以下に XML のメタデータファイルを生成します。

.. tip::

    上記のコマンドで、最初の引数を `yml` に変更すれば、メタデータのクラスを YAML フォーマットで生成することもできます。

生成された ``BlogPost.dcm.xml`` メタデータファイルは以下のようになります。
:

.. code-block:: xml

    <?xml version="1.0" encoding="utf-8"?>
    <doctrine-mapping>
      <entity name="BlogPost" table="blog_post">
        <change-tracking-policy>DEFERRED_IMPLICIT</change-tracking-policy>
        <id name="id" type="bigint" column="id">
          <generator strategy="IDENTITY"/>
        </id>
        <field name="title" type="string" column="title" length="100"/>
        <field name="content" type="text" column="content"/>
        <field name="isPublished" type="boolean" column="is_published"/>
        <field name="createdAt" type="datetime" column="created_at"/>
        <field name="updatedAt" type="datetime" column="updated_at"/>
        <field name="slug" type="string" column="slug" length="255"/>
        <lifecycle-callbacks/>
      </entity>
    </doctrine-mapping>

メタデータファイルが生成されれば、 Doctrine にスキーマをインポートし、関連するエンティティクラスを作成するように尋ねることができます。次のコマンドを実行してください。

.. code-block:: bash

    php app/console doctrine:mapping:import AcmeBlogBundle annotation
    php app/console doctrine:generate:entities AcmeBlogBundle

最初のコマンドは、アノテーションマッピングをしたエンティティクラスを生成します。もちろん ``anotation`` 引数ではなく、 ``xml`` や ``yml`` に変更することができます。新しく作成された ``BlogComment`` エンティティクラスは以下のようになります。
:

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
         * @var bigint $id
         *
         * @ORM\Column(name="id", type="bigint", nullable=false)
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

最後のコマンドは、 ``BlogPost`` と ``BlogComment`` エンティティクラスのプロパティの全てのゲッターとセッターを生成します。生成されたエンティティは、これで使用することができました。

.. _`Doctrine tools documentation`: http://www.doctrine-project.org/docs/orm/2.0/en/reference/tools.html#reverse-engineering

.. 2011/11/01 ganchiku d739c578e765de86a8ad54d6ce9cd32b8a098a1f

