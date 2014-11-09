セキュリティ
========

認証とファイアウォール (i.e ユーザの権限情報を取得する)
------------------------------------------------------------------

任意の方法を使用してユーザー認証を行い、
任意のソースからユーザー情報を読み込むように Symfony を設定することができます。

セキュリティはとても難解なテーマですが、`Security Cookbook Section`_ に多くの情報が記載されています。

認証は、その必要性の有無に関わらず、
``security.yml`` の ``firewalls`` キーの下に設定されています。

.. best-practice::

    正規に許可された２つの異なる認証システムとユーザが無い限り
    (e.g. メインとなるサイトと API のためだけのトークンシステムのためのログイン)、
    ``anonymous`` のキーを有効にしたたったひとつのファイヤーウォールを設けることを推奨します。

ほとんどのアプリケーションで認証システムとユーザのセットはひとつしかありません。
このため、ひとつのファイヤーウォールの設定だけで事足ります。
もちろん、サイトの API と WEB を分けたい場合などの例外もありますが、シンプルに考えていくことが重要です。

なお、ファイヤーウォールの下では ``anonymous`` キーを利用するべきです。
サイトの異なるセクション (または 全てのセクションのようなもの) にユーザが
ログインすることが必要となった場合は ``access_control`` エリアを利用します。

.. best-practice::

    ユーザのパスワードのエンコーディングには ``bcrypt`` エンコーダーを利用する。

ユーザがパスワードを持つ場合、SHA-512 の代わりに ``bcrypt`` エンコーダーを利用することを推奨します。
``bcrypt`` の主要なアドバンテージは、**ソルト** の値が有しているものが
レインボーテーブルアタックから保護しブルートフォースアタックを遅延できることです。

この考えで、データベースからユーザをロードするログインフォームを利用する認証を
アプリケーションにセットアップする設定は以下となります。

.. code-block:: yaml

    security:
        encoders:
            AppBundle\Entity\User: bcrypt

        providers:
            database_users:
                entity: { class: AppBundle:User, property: username }

        firewalls:
            secured_area:
                pattern: ^/
                anonymous: true
                form_login:
                    check_path: security_login_check
                    login_path: security_login_form

                logout:
                    path: security_logout
                    target: homepage

    # ... access_control exists, but is not shown here

.. tip::

    プロジェクトのためのソースコードはそれぞれの部分を説明するコメントを含む。

承認 (i.e. アクセスを拒否すること)
-----------------------------------

Symfony は承認のためにいくつかの方法を提供しています。
`security.yml`_ に ``access_control`` の設定を含めること、
``security.context`` サービスに直接 `@Security annotation <best-practices-security-annotation>` と
`isGranted <best-practices-directy-isGranted>` を使うという方法です。

.. best-practice::

    * 広範囲の URL パターンをプロテクトするためには ``access_control`` を利用する。
    * いつでも可能なときは ``@Security`` アノテーションを利用する。
    * より複雑な状況の場合はいつでも、
      ``security.context`` サービスでセキュリティを直接的にチェックする。

承認ロジックを一箇所に集中させるには
カスタムセキュリティ Voter や ACL を利用するというような方法があります。

.. best-practice::

    * きめ細かい制限のために、カスタムセキュリティ Voter を定義する。
    * 管理機能を経由したあらゆるユーザによる
      あらゆるのオブジェクトへのアクセスを制限するために Symfony ACL を利用する。

.. _best-practices-security-annotation:

@Security アノテーション
------------------------

可能な限り、コントローラーごとにアクセスを制御するためには ``@Secury`` アノテーションを利用します。
この記述は、可読性が高く、整合性を保ったままでそれぞれのアクションに設置することができます。

このアプリケーションでは、新しいポストを作成するための ``ROLE_ADMIN`` が必要です。
``@Security`` を利用するとこのようになります。

.. code-block:: php

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Security;
    // ...

    /**
     * Displays a form to create a new Post entity.
     *
     * @Route("/new", name="admin_post_new")
     * @Security("has_role('ROLE_ADMIN')")
     */
    public function newAction()
    {
        // ...
    }

Expression による複雑なセキュリティの制限
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

セキュリティロジックがかなり複雑な場合は、
``@Security`` の内部で `expression`_ を利用できます。
例えば、
``Post`` オブジェクトの ``getAuthorEmail`` メソッドの返り値とメールアドレスが一致したときのみ
コントローラーへのアクセスを許可したい場合は以下のように実装できます。

.. code-block:: php

    use AppBundle\Entity\Post;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Security;

    /**
     * @Route("/{id}/edit", name="admin_post_edit")
     * @Security("user.getEmail() == post.getAuthorEmail()")
     */
    public function editAction(Post $post)
    {
        // ...
    }

``Post`` オブジェクトとそれに与えられる ``$post`` という引数を自動的に取得する
ために `PramConverter`_ の利用が必須であることに注意してください。

この方法にはよく知られた欠点があります。
アノテーションによる記述は、アプリケーションの他の部分で簡単に再利用できません。
投稿の著者のみが閲覧できるテンプレートのリンクを追加したい場合を想像してください。
Twig の文法を利用して再び記述する必要があることがすぐに思い浮かぶでしょう。

.. code-block:: html+jinja

    {% if app.user and app.user.email == post.authorEmail %}
        <a href=""> ... </a>
    {% endif %}

十分にシンプルなロジックの場合、最も簡単な解決手段は
与えられたユーザがその投稿の著者であるかをチェックする新しい関数を ``Post`` エンティティに追加することです。

.. code-block:: php

    // src/AppBundle/Entity/Post.php
    // ...

    class Post
    {
        // ...

        /**
         * Is the given User the author of this Post?
         *
         * @return bool
         */
        public function isAuthor(User $user = null)
        {
            return $user && $user->getEmail() == $this->getAuthorEmail();
        }
    }

これで、この関数をテンプレートとセキュリティの記述のどちらでも再利用できます。

.. code-block:: php

    use AppBundle\Entity\Post;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Security;

    /**
     * @Route("/{id}/edit", name="admin_post_edit")
     * @Security("post.isAuthor(user)")
     */
    public function editAction(Post $post)
    {
        // ...
    }

.. code-block:: html+jinja

    {% if post.isAuthor(app.user) %}
        <a href=""> ... </a>
    {% endif %}

.. _best-practices-directy-isGranted:

@Security を利用しない権限のチェック
--------------------------------------

これまでの例は、
``post`` という変数にアクセスできる記述を提供してくれる :ref:`ParamConverter <best-practices-paramconverter>` を
利用する場合のみ動作します。
これを利用しない場合やより応用的なユースケースの場合は、PHP で同様のセキュリティチェックができます。

.. code-block:: php

    /**
     * @Route("/{id}/edit", name="admin_post_edit")
     */
    public function editAction($id)
    {
        $post = $this->getDoctrine()->getRepository('AppBundle:Post')
            ->find($id);

        if (!$post) {
            throw $this->createNotFoundException();
        }

        if (!$post->isAuthor($this->getUser())) {
            throw $this->createAccessDeniedException();
        }

        // ...
    }

セキュリティ Voter
---------------

セキュリティロジックが複雑で ``isAuthor()`` のようなメソッドに局所化できない場合、
カスタム Voter を利用することができます。
これらは `ACL's`_ よりもかなり簡単な方法かつほぼ全てのケースに柔軟に対応できます。

まずはじめに、Voter クラスを作成します。以下の例では、これまでと同じ ``getAuthorEmail`` ロジックを
実装した Voter について示します。

.. code-block:: php

    namespace AppBundle\Security;

    use Symfony\Component\Security\Core\Authorization\Voter\AbstractVoter;
    use Symfony\Component\Security\Core\User\UserInterface;

    // AbstractVoter class requires Symfony 2.6 or higher version
    class PostVoter extends AbstractVoter
    {
        const CREATE = 'create';
        const EDIT   = 'edit';

        protected function getSupportedAttributes()
        {
            return array(self::CREATE, self::EDIT);
        }

        protected function getSupportedClasses()
        {
            return array('AppBundle\Entity\Post');
        }

        protected function isGranted($attribute, $post, $user = null)
        {
            if (!$user instanceof UserInterface) {
                return false;
            }

            if ($attribute === self::CREATE && in_array('ROLE_ADMIN', $user->getRoles(), true)) {
                return true;
            }

            if ($attribute === self::EDIT && $user->getEmail() === $post->getAuthorEmail()) {
                return true;
            }

            return false;
        }
    }

アプリケーションでセキュリティ Voter を有効にするために新しいサービスを定義します。

.. code-block:: yaml

    # app/config/services.yml
    services:
        # ...
        post_voter:
            class:      AppBundle\Security\PostVoter
            public:     false
            tags:
               - { name: security.voter }

ここで ``@Security`` アノテーションに Voter を利用してみます。

.. code-block:: php

    /**
     * @Route("/{id}/edit", name="admin_post_edit")
     * @Security("is_granted('edit', post)")
     */
    public function editAction(Post $post)
    {
        // ...
    }

これを直接 ``security.context`` と一緒に使うことも可能です。
また、コントローラーでより簡潔なショートカットを経由することも可能です。

.. code-block:: php

    /**
     * @Route("/{id}/edit", name="admin_post_edit")
     */
    public function editAction($id)
    {
        $post = // query for the post ...

        if (!$this->get('security.context')->isGranted('edit', $post)) {
            throw $this->createAccessDeniedException();
        }
    }

さらに学ぶためには
----------

Symfony のコミュニティによって開発された `FOSUserBundle`_ は Symfony2 における
データベースによるユーザシステムのサポートを追加しました。
これはユーザの登録やパスワードを忘れた時の機能などの共通のタスクも取り扱っています。

`Remember Me feature`_ を有効にすることによってユーザを長期期間ログインさせておくことも可能です。

カスタマーサポートを提供したとき、問題を再現するために異なるユーザでアプリケーションにアクセス
することがたびたび必要になります。
Symfony は `impersonate users`_ の機能を提供しています。

Symfony でサポートしていないユーザのログインメソッドを利用する場合、
`your own user provider`_ と `your own authentication provider`_ で開発することができます。

.. _`Security Cookbook Section`: http://symfony.com/doc/current/cookbook/security/index.html
.. _`security.yml`: http://symfony.com/doc/current/reference/configuration/security.html
.. _`ParamConverter`: http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/annotations/converters.html
.. _`@Security annotation`: http://symfony.com/doc/current/bundles/SensioFrameworkExtraBundle/annotations/security.html
.. _`security.yml`: http://symfony.com/doc/current/reference/configuration/security.html
.. _`security voter`: http://symfony.com/doc/current/cookbook/security/voters_data_permission.html
.. _`Acces Control List`: http://symfony.com/doc/current/cookbook/security/acl.html
.. _`ACL's`: http://symfony.com/doc/current/cookbook/security/acl.html
.. _`expression`: http://symfony.com/doc/current/components/expression_language/introduction.html
.. _`FOSUserBundle`: https://github.com/FriendsOfSymfony/FOSUserBundle
.. _`Remember Me feature`: http://symfony.com/doc/current/cookbook/security/remember_me.html
.. _`impersonate users`: http://symfony.com/doc/current/cookbook/security/impersonating_user.html
.. _`your own user provider`: http://symfony.com/doc/current/cookbook/security/custom_provider.html
.. _`your own authentication provider`: http://symfony.com/doc/current/cookbook/security/custom_authentication_provider.html
