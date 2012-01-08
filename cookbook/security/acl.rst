.. index::
   single: Security; Access Control Lists (ACLs)

アクセス制御リスト(ACLs)
========================

複雑なアプリケーションでは、アクセスしてきたユーザ(``Token``)のみに基づいてアクセスの可否を判断することができないこともあります。アクセスの可否の判断は、アクセスを行ったドメインオブジェクトが関わるときもあります。こういったときに ACL システムが必要になります。

ブログの設計をしていて、ユーザが投稿にコメントができるようにしたいとしましょう。そしてユーザに他人のコメントは編集できないが、自分のコメントを編集できるようにしたいとします。また、ブログの管理者であるあなたには全てのコメントを編集できるようにしたいとします。このシナリオでは、 ``Comment`` はアクセスを制限したいドメインオブジェクトになります。 Symfony2 を使用すれば、いろんなアプローチでこれを実現できますが、次の２つが一般的です。

- *ビジネスロジックのセキュリティを強化する*: このアプローチでは、個々の  ``Comment`` 内にアクセス権を持つユーザを参照できるようにしておき、ユーザの持つ ``Token`` を比較します。
- *権限のセキュリティを強化する*: このアプローチでは、個々の ``Comment`` オブジェクトに権限を追加します。例えば、 ``ROLE_COMMENT_1`` や ``ROLE_COMMENT_2`` のようにです。

両方のアプローチは、どちらも有効です。しかし、認証のロジックとビジネスロジックのコードの結合が密になってしまい、他で再利用しにくくなりますし、ユニットテストもしにくくなります。さらに、ユーザの数が多ければ、１つのドメインオブジェクトにアクセスが増えてしまい、パフォーマンス悪化の問題にもつながります。

幸運にも、上記の方法よりも良い方法がありますので、これから説明をしていきます。

ブートストラップをする
----------------------

まず始める前に、ブートストラップをしましょう。まず、 ACL システムが使用する接続を設定する必要があります。

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            acl:
                connection: default

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <acl>
            <connection>default</connection>
        </acl>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', 'acl', array(
            'connection' => 'default',
        ));


.. note::

    ACL システムを使用するには、少なくとも１つは Doctrine DBAL 接続を設定しておく必要があります。しかし、ドメインオブジェクトをマップするのに、 Doctrine を使わなければならないわけではありません。オブジェクトのマッパーには、好きなものを使用できますので、 Doctrine ORM,  Mongo ODM,  Propel,  生の SQL など選択ができます。

接続を設定したら、データベース構造をインポートします。幸運にも、次のコマンドを走らせることによって、これを行うことができます。

.. code-block:: text

    php app/console init:acl

開始する
--------

最初の小さな例に戻って、 ACL を実装しましょう。

ACL を作成して、 ACE を追加する
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: php

    use Symfony\Component\Security\Core\Exception\AccessDeniedException;
    use Symfony\Component\Security\Acl\Domain\ObjectIdentity;
    use Symfony\Component\Security\Acl\Domain\UserSecurityIdentity;
    use Symfony\Component\Security\Acl\Permission\MaskBuilder;
    // ...
    
    // BlogController.php
    public function addCommentAction(Post $post)
    {
        $comment = new Comment();

        // setup $form, and bind data
        // ...

        if ($form->isValid()) {
            $entityManager = $this->get('doctrine.orm.default_entity_manager');
            $entityManager->persist($comment);
            $entityManager->flush();

            // creating the ACL
            $aclProvider = $this->get('security.acl.provider');
            $objectIdentity = ObjectIdentity::fromDomainObject($comment);
            $acl = $aclProvider->createAcl($objectIdentity);

            // retrieving the security identity of the currently logged-in user
            $securityContext = $this->get('security.context');
            $user = $securityContext->getToken()->getUser();
            $securityIdentity = UserSecurityIdentity::fromAccount($user);

            // grant owner access
            $acl->insertObjectAce($securityIdentity, MaskBuilder::MASK_OWNER);
            $aclProvider->updateAcl($acl);
        }
    }

上記のコードスニペットには、いくつかの重要な実装があります。ここでは、それに関して２つ取り上げましょう。

まず、 ``->createAcl()`` メソッドの呼び出しが直接ドメインオブジェクトではなく ``ObjectIdentityInterface`` の実装のみを受け取っていることに気づくでしょう。間接的な受け渡しによって、ドメインオブジェクトのインスタンスを持っていなくても、 ACL を操作することができます。実際にこれらのオブジェクトをハイドレートしなくても多くのオブジェクトのパーミッションをチェックできるので、非常に便利です。

また、 ``->insertObjectAce()`` メソッドの呼び出しも興味深い点です。この例では、現在ログインしているユーザに Comment へのオーナーアクセスを与えています。 ``MaskBuilder::MASK_OWNER`` は、整数値のビットマスク(bitmask) として前もって定義してあります。マスクビルダー(MaskBuilder)は、ほとんどの技術的な詳細を抽象化しており、この技術を使えば、多くの異なるパーミッションを１列に格納するだけなので、パフォーマンスの改善の享受ができるようになっています。

.. tip::

    ACE がチェックされる順番は重要です。一般的に、始めにより特定したエントリを入れることが推奨されます。

アクセスをチェックする
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: php

    // BlogController.php
    public function editCommentAction(Comment $comment)
    {
        $securityContext = $this->get('security.context');

        // check for edit access
        if (false === $securityContext->isGranted('EDIT', $comment))
        {
            throw new AccessDeniedException();
        }

        // retrieve actual comment object, and do your editing here
        // ...
    }

この例では、ユーザが ``EDIT`` パーミッションを持ってるかどうかチェックしています。内部的に Symfony2 は、パーミッションをいくつもの整数値のビットマスク(bitmask)にマップしており、ユーザの持ってるパーミッションをチェックします。

.. note::

    ３２ものパーミッションを定義することができます(動作している OS の PHP によって３０もしくは３２になります)。さらに累積(cumulative)パーミッションも定義することができます。

累積(cumulative)パーミッション
------------------------------

上記の最初の例では、ユーザに ``OWNER`` パーミッションを与えるのみでした。確かにこれでユーザに対して、ドメインオブジェクトの参照、編集などのオペレーションを行うことを可能にさせることができます。しかし、より明示的にこれらのパーミッションを与たいときなどもあると思います。

``MaskBuilder`` を使って、パーミッションのベースを結合させることで簡単にビットマスク(bitmask)を作成することができます。

.. code-block:: php

    $builder = new MaskBuilder();
    $builder
        ->add('view')
        ->add('edit')
        ->add('delete')
        ->add('undelete')
    ;
    $mask = $builder->get(); // int(15)

整数値のビットマスク(bitmask)は、上記でユーザに追加したパーミッションとして使われます。

.. code-block:: php

    $acl->insertObjectAce(new UserSecurityIdentity('johannes'), $mask);

これでユーザはオブジェクトに対して、参照、編集、削除、そして削除の取り消しができるようになりました。

.. 2011/11/11 ganchiku df8200965642b6897d77fd4069077520dbcc70a9

