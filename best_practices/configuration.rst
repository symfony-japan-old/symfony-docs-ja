設定
=============

設定は、大抵、アプリケーション内の別々の部分（例えば、インフラとユーザー権限のような）と別々の環境
（開発環境、プロダクション環境）に関連しています。
そこで、 Symfony はアプリケーションの設定を3つの部分に分けて考えるようにしています。

インフラに関係する設定
------------------------------------

.. best-practice::

    インフラに関係する設定オプションは ``app/config/parameters.yml`` ファイルに定義しましょう。

デフォルトの ``parameters.yml`` ファイルはそうなっており、データベースとメールサーバーの設定を定義しています。

.. code-block:: yaml

    # app/config/parameters.yml
    parameters:
        database_driver:   pdo_mysql
        database_host:     127.0.0.1
        database_port:     ~
        database_name:     symfony
        database_user:     root
        database_password: ~

        mailer_transport:  smtp
        mailer_host:       127.0.0.1
        mailer_user:       ~
        mailer_password:   ~

        # ...


このオプションは ``app/config/config.yml`` では定義されていません。というのも、アプリケーションの振る舞いに
全く関係がないからです。
つまり、アプリケーションは、オプションが正しく設定されている限りでは、データベースの場所やユーザー名や
パスワードに関心を持たないのです。

デフォルトのパラメータ
~~~~~~~~~~~~~~~~~~~~~~~~

.. best-practice::

    アプリケーションの全てのパラメータを ``app/config/parameters.yml.dist`` に定義しましょう。

バージョン2.3以降、 Symfony には ``parameters.yml.dist`` という設定ファイルが含まれており、アプリケーションの
設定で使われるデフォルトのパラメーターリストが保存されています。

新しい設定パラメータを定義したなら、このファイルにもそのパラメータを追加して、バージョン管理に反映させましょう。
開発者がプロジェクトをアップデートした時やサーバーにデプロイした時に、 デフォルトの ``parameters.yml.dist`` と
ローカルの ``parameters.yml`` の差分の有無を Symfony が自動的に調べます。
もし差分があれば、 Symfony は新しいパラメータの値を尋ね、その値を ``parameters.yml`` に追記します。

アプリケーションに関する設定
---------------------------------

.. best-practice::

    アプリケーションの振る舞いに関する設定は ``app/config/config.yml`` に定義しましょう。


``config.yml`` ファイルには、メール通知の送信元や `feature toggles`_ のような、アプリケーションの振る舞いを
変える設定が含まれます。
このようなオプション値を ``parameters.yml`` ファイルに定義してしまうと、サーバーごとに変える必要はない設定まで
サーバーごとに設定しなかればならなくなります。

``config.yml`` に定義されている設定オプションは、大抵、 `execution environment`_ によって変わります。
そこで、 Symfony には ``app/config/config_dev.yml`` と ``app/config/config_prod.yml`` があり、環境ごとに
別の値を設定できるようになっているのです。

定数か、設定オプションか
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

アプリケーションの設定をするときに最もありがちなミスは、ページ分割の1ページあたりの件数のような、滅多に
変更しない値をオプションとして定義することです。

.. best-practice::

    滅多に変更しないオプションは定数として定義しましょう。

設定オプションを定義する伝統的な方法の結果として、多くの Symfony アプリケーションで下のようなオプションが
定義されています。ブログホームページに表示する投稿の数を制御するオプション値です。

.. code-block:: yaml

    # app/config/config.yml
    parameters:
        homepage.num_items: 10

この類いのオプションを最後に変更したのはいつだったか自問してみると、おそらく *一度も変更したことがない* という
答えが出るでしょう。変更しない設定を設定オプションにするのは無駄です。このような値は定数として定義するのが良い
でしょう。
例えば、 ``Post`` エンティティの ``NUM_ITEMS`` 定数として定義するのです。

.. code-block:: php

    // src/AppBundle/Entity/Post.php
    namespace AppBundle\Entity;

    class Post
    {
        const NUM_ITEMS = 10;

        // ...
    }

定数として定義する方式の主なメリットは、値をアプリケーション内のどこでも利用できることです。パラメータとして
定義してしまうと、 Symfony のDIコンテナにアクセスできる場所でしか値を利用できませんでした。

例えば、定数は ``constant()`` ヘルパーを利用して Twig テンプレートの中でも使うことができます、

.. code-block:: html+jinja

    <p>
        最新の投稿から {{ constant('NUM_ITEMS', post) }} 件表示しています。
    </p>

コンテナのパラメータを取得できない Doctrine のエンティティやレポジトリからも値を利用できます。

.. code-block:: php

    namespace AppBundle\Repository;

    use Doctrine\ORM\EntityRepository;
    use AppBundle\Entity\Post;

    class PostRepository extends EntityRepository
    {
        public function findLatest($limit = Post::NUM_ITEMS)
        {
            // ...
        }
    }

定数で設定を定義することの唯一の弱点として、テストの時に簡単に値を上書きすることができないので、注意してください。

セマンティックな設定（利用してはいけません）
--------------------------------------------

.. best-practice::

    バンドルの設定をセマンティックなDI設定として定義しないようにしましょう。    

`How to Expose a semantic Configuration for a Bundle`_ で説明されているように、 Symfony のバンドルには設定の方法が
2つあります。 ``services.yml`` を使った通常の設定と、特別な ``*Extension`` クラスを使ったセマンティックな設定です。

セマンティックな設定は強力で、設定項目のバリデーションのような役立つ機能もありますが、セマンティック設定を定義する
のにかかる作業が多すぎて、サードパーティのバンドルとして再利用されないバンドルを作る場合には釣り合いが取れないのです。

センシティブなオプションを完全に Symfony の外に追い出す
--------------------------------------------------------

データベースの接続情報のようなセンシティブなオプションを設定するときには、 Symfony のプロジェクトの外部に保存し、
環境変数を利用して読み込む方式も推奨します。
この方式をどのようにして実現するのかについては、 `How to Set external Parameters in the Service Container`_ を
参照してください。

.. _`feature toggles`: http://en.wikipedia.org/wiki/Feature_toggle
.. _`execution environment`: http://symfony.com/doc/current/cookbook/configuration/environments.html
.. _`constant() function`: http://twig.sensiolabs.org/doc/functions/constant.html
.. _`How to Expose a semantic Configuration for a Bundle`: http://symfony.com/doc/current/cookbook/bundles/extension.html
.. _`How to Set external Parameters in the Service Container`: http://symfony.com/doc/current/cookbook/configuration/external_parameters.html
