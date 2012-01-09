.. index::
   single: Security; User Provider
   single: Security; Entity Provider

データベースからセキュリティユーザをロードする方法(エンティティプロバイダ)
==========================================================================

Symfony の最もスマートツールの１つは、セキュリティレイヤーです。セキュリティレイヤーは、認証と承認のプロセスを処理します。内部的の動作を理解することは難しそうに見えます。しかし、セキュリティシステムはとても柔軟で、アクティブディレクトリ、OAuth認証、データベース認証などいろんな認証を統合することができるようになっています。

イントロダクション
------------------

この記事では、 Doctrine のエンティティクラスによるデータベーステーブルに対するユーザ認証にフォーカスを当ててみます。このレシピの内容は、３つに別れています。まず第１のパートでは、 Doctrine ``User`` エンティティクラスを設計して、 Symfony のセキュリティレイヤーで利用可能にすることに関して説明します。第２のパートでは、 Doctrine の :class:`Symfony\\Bridge\\Doctrine\\Security\\User\\EntityUserProvider` オブジェクトとそのコンフィギュレーションを使用して、簡単にユーザ認証を行う方法を説明します。最後のパートでは、:class:`Symfony\\Bridge\\Doctrine\\Security\\User\\EntityUserProvider` のカスタムクラスを作成し、カスタム化した条件でデータベースからユーザを検索する方法を説明します。

このチュートリアルでは、アプリケーションカーネルで ``Acme\UserBundle`` バンドルをブートストラップしロードしてあることを想定います。

データモデル
------------

このレシピの目的のために、 ``AcmeUserBundle`` バンドルが ``User`` エンティティクラスを含んでおり、 ``User`` エンティティには、 ``id``, ``username``, ``salt``, ``password``, ``email``, ``isActive`` フィールドを持つようにしてください。 ``isActive`` フィールドはユーザアカウントが有効化どうかのフラグです。

短くするために、ゲッターメソッドとセッターメソッドを削除して、 :class:`Symfony\\Component\\Security\\Core\\User\\UserInterface` インタフェースの重要なメソッドにフォーカスしています。

.. code-block:: php

    // src/Acme/UserBundle/Entity/User.php

    namespace Acme\UserBundle\Entity;

    use Symfony\Component\Security\Core\User\UserInterface;
    use Doctrine\ORM\Mapping as ORM;

    /**
     * Acme\UserBundle\Entity\User
     *
     * @ORM\Table(name="acme_users")
     * @ORM\Entity(repositoryClass="Acme\UserBundle\Entity\UserRepository")
     */
    class User implements UserInterface
    {
        /**
         * @ORM\Column(name="id", type="integer")
         * @ORM\Id()
         * @ORM\GeneratedValue(strategy="AUTO")
         */
        private $id;

        /**
         * @ORM\Column(name="username", type="string", length=25, unique=true)
         */
        private $username;

        /**
         * @ORM\Column(name="salt", type="string", length=40)
         */
        private $salt;

        /**
         * @ORM\Column(name="password", type="string", length=40)
         */
        private $password;

        /**
         * @ORM\Column(name="email", type="string", length=60, unique=true)
         */
        private $email;

        /**
         * @ORM\Column(name="is_active", type="boolean")
         */
        private $isActive;

        public function __construct()
        {
            $this->isActive = true;
            $this->salt = base_convert(sha1(uniqid(mt_rand(), true)), 16, 36);
        }

        public function getRoles()
        {
            return array('ROLE_USER');
        }

        public function equals(UserInterface $user)
        {
            return $user->getUsername() === $this->username;
        }

        public function eraseCredentials()
        {
        }

        public function getUsername()
        {
            return $this->username;
        }

        public function getSalt()
        {
            return $this->salt;
        }

        public function getPassword()
        {
            return $this->password;
        }
    }

Symfony のセキュリティレイヤーで ``AcmeUserBundle:User`` クラスのインスタンスを利用するために、エンティティクラスは、 :class:`Symfony\\Component\\Security\\Core\\User\\UserInterface` インタフェースを実装する必要があります。このインタフェースは、次の６つのメソッドを実装を強制します。それは、 ``getRolls()``, ``getPassword()``, ``getSalt()``, ``getUsername()``, ``eraseCredentials()``, ``equals()`` です。それぞれのメソッドの詳細は、 :class:`Symfony\\Component\\Security\\Core\\User\\UserInterface` を参照してください。

説明を簡単にするために、 ``equals()`` メソッドでは、 ``username`` フィールドを比較するのみにしています。しかし、もちろんあなたのデータモデルの複雑性に応じて、より多くチェックすることもできます。 ``eraseCredentials()`` メソッドでは、この記事では、重要でないため空のままとしています。

以下が、 MySQL の ``User`` テーブルの内容の一例です。ユーザレコードの作成やパスワードのエンコードの方法の詳細は、  :ref:`book-security-encoding-user-password` を参照してください。

.. code-block:: text

    mysql> select * from user;
    +----+----------+------------------------------------------+------------------------------------------+--------------------+-----------+
    | id | username | salt                                     | password                                 | email              | is_active |
    +----+----------+------------------------------------------+------------------------------------------+--------------------+-----------+
    |  1 | hhamon   | 7308e59b97f6957fb42d66f894793079c366d7c2 | 09610f61637408828a35d7debee5b38a8350eebe | hhamon@example.com |         1 |
    |  2 | jsmith   | ce617a6cca9126bf4036ca0c02e82deea081e564 | 8390105917f3a3d533815250ed7c64b4594d7ebf | jsmith@example.com |         1 |
    |  3 | maxime   | cd01749bb995dc658fa56ed45458d807b523e4cf | 9764731e5f7fb944de5fd8efad4949b995b72a3c | maxime@example.com |         0 |
    |  4 | donald   | 6683c2bfd90c0426088402930cadd0f84901f2f4 | 5c3bcec385f59edcc04490d1db95fdb8673bf612 | donald@example.com |         1 |
    +----+----------+------------------------------------------+------------------------------------------+--------------------+-----------+
    4 rows in set (0.00 sec)

テーブルには、４つのユーザが異なる username,  email で入っています。次の節では、 Doctrine エンティティユーザプロバイダを使用し、設定をして、これらのユーザの認証方法に着目します。

データベースでユーザを承認する
------------------------------

データベースに対して Doctrine のユーザを Symfony のセキュリティレイヤーで認証することはとても簡単です。 ``app/config/security.yml`` ファイルで :doc:`SecurityBundle</reference/configuration/security>` 設定を全てすることができるのです。

下記は、 HTTP ベーシック認証での username と password を入力するユーザの設定の例です。これらの情報は、データベースのユーザエンティティのレコードでチェックされます。


.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            encoders:
                Acme\UserBundle\Entity\User:
                    algorithm: sha1
                    encode_as_base64: false
                    iterations: 1

            providers:
                administrators:
                    entity: { class: AcmeUserBundle:User, property: username }

            firewalls:
                admin_area:
                    pattern:    ^/admin
                    http_basic: ~

            access_control:
                - { path: ^/admin, roles: ROLE_ADMIN }

``encoders`` セクションは、エンティティクラスの ``sha1`` パスワードエンコーダーに関連付けています。これは、 Symfony がデータベースに保存するパスワードが ``sha1`` のアルゴリズムを使用してエンコードされるようにしています。正しくパスワードをエンコードして、新しくユーザオブジェクトを作成する方法の詳細は、セキュリティの章の :ref:`book-security-encoding-user-password` セクションを参照してください。

``providers`` セクションは、 ``administrators`` ユーザプロバイダを定義します。ユーザプロバイダは、認証の際にユーザがロードされる "source" になります。今回のケースでは、 ``entity`` キーワードは、次のことを意味いています。それは、ユニークなフィールドの ``username`` を使用して、データベースからユーザエンティティオブジェクトを検索するのに、 Symfony が Doctrine エンティティユーザプロバイダを使用するということです。つまり、これで Symfony がパスワードの妥当性をチェックする前いデータベースからユーザを取ってくることを意味いているのです。

このコードと設定で動作はしますが、 **有効** ユーザのアプリケーションをセキュアにするには不十分です。ですので、依然 ``maxime`` で認証できてしまいます。次のセクションでは、無効なユーザを拒否する方法を説明します。

無効なユーザを拒否する
----------------------

無効なユーザを除外する最も簡単な方法は、ユーザアカウントの状態をチェックする :class:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface` インタフェースを実装することです。  :class:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface` インタフェースは、 :class:`Symfony\\Component\\Security\\Core\\User\\UserInterface` インタフェースを拡張しているので、 ``AcmeUserBundle:User`` エンティティクラス内で新しいインタフェースをスイッチするだけでシンプルで高度な認証の仕組みの恩恵を受けることができます。

:class:`Symfony\\Component\\Security\\Core\\User\\AdvancedUserInterface` インタフェースは、アカウントの状態をバリデートするために、次の４つのメソッドを追加しています。

* ``isAccountNonExpired()`` ユーザアカウントが期限切れになっているかチェックします
* ``isAccountNonLocked()`` ユーザがロックされているかチェックします
* ``isCredentialsNonExpired()`` ユーザの証明 (パスワード)が期限切れなっているかチェックします
* ``isEnabled()`` ユーザが有効かチェックします

この例では、最初の３つのメソッドは、 ``true`` を返しますが、 ``isEnabled()`` メソッドは、 ``isActive`` フィールドの boolean の値を返しています。

.. code-block:: php

    // src/Acme/UserBundle/Entity/User.php

    namespace Acme\Bundle\UserBundle\Entity;

    // ...
    use Symfony\Component\Security\Core\User\AdvancedUserInterface;

    // ...
    class User implements AdvancedUserInterface
    {
        // ...
        public function isAccountNonExpired()
        {
            return true;
        }

        public function isAccountNonLocked()
        {
            return true;
        }

        public function isCredentialsNonExpired()
        {
            return true;
        }

        public function isEnabled()
        {
            return $this->isActive;
        }
    }

これで ``maxime`` で認証しようとすれば、有効なアカウントではないので、アクセスは拒否されます。次のセクションでは、 username や email での認証を行うカスタムエンティティプロバイダの書き方に焦点を宛てます。

カスタムエンティティプロバイダで認証を行う
------------------------------------------

次のステップは、データベース内でユニークである username や email でユーザを認証させます。残念ながらネイティブのエンティティプロバイダは、データベースからユーザを取り出す際に、１つのプロパティしか処理することができません。

これを実現するために、サブミットされたログイン username が username *もしくは* email フィールドがマッチするかをチェックするカスタムエンティティプロバイダを作成します。幸いなことに、 :class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface`. インタフェースを実装すれば、 Doctrine リポジトリオブジェクトは、エンティティユーザプロバイダとして振る舞うことができます。このインタフェースでは次の３つのメソッドを強制します。 ``loadUserByUsername($username)``, ``refreshUser(UserInterface $user)``, ``supportsClass($class)`` です。詳細は、 :class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface` を参照してください。

以下のコードは、 ``UserRepository`` クラス内の :class:`Symfony\\Component\\Security\\Core\\User\\UserProviderInterface` の実装になります。
::

    // src/Acme/UserBundle/Entity/UserRepository.php

    namespace Acme\UserBundle\Entity;

    use Symfony\Component\Security\Core\User\UserInterface;
    use Symfony\Component\Security\Core\User\UserProviderInterface;
    use Symfony\Component\Security\Core\Exception\UsernameNotFoundException;
    use Symfony\Component\Security\Core\Exception\UnsupportedUserException;
    use Doctrine\ORM\EntityRepository;
    use Doctrine\ORM\NoResultException;

    class UserRepository extends EntityRepository implements UserProviderInterface
    {
        public function loadUserByUsername($username)
        {
            $q = $this
                ->createQueryBuilder('u')
                ->where('u.username = :username OR u.email = :email')
                ->setParameter('username', $username)
                ->setParameter('email', $username)
                ->getQuery()
            ;

            try {
                // The Query::getSingleResult() method throws an exception
                // if there is no record matching the criteria.
                $user = $q->getSingleResult();
            } catch (\NoResultException $e) {
                throw new UsernameNotFoundException(sprintf('Unable to find an active admin AcmeUserBundle:User object identified by "%s".', $username), null, 0, $e);
            }

            return $user;
        }

        public function refreshUser(UserInterface $user)
        {
            $class = get_class($user);
            if (!$this->supportsClass($class)) {
                throw new UnsupportedUserException(sprintf('Instances of "%s" are not supported.', $class));
            }

            return $this->loadUserByUsername($user->getUsername());
        }

        public function supportsClass($class)
        {
            return $this->getEntityName() === $class || is_subclass_of($class, $this->getEntityName());
        }
    }

実装を終えるには、セキュリティレイヤーの設定を変更して、Symfony に、最初から入ってる値の Doctrine エンティティプロバイダではなく、今回作成したカスタムエンティティプロバイダを使用するように変更する必要があります。 ``security.yml`` ファイルの  ``security.providers.administrators.entity`` セクション内の ``property`` フィールドを削除するだけです。
 (It's trival to achieve by removing the ``property`` field in the ``security.providers.administrators.entity`` section of the ``security.yml`` file.)

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            providers:
                administrators:
                    entity: { class: AcmeUserBundle:User }
            # ...

これで、セキュリティレイヤーは、 ``UserRepository`` のインスタンスを使用して ``loadUserByUsername()`` メソッドを呼ぶようになり、 username でも email でもデータベースからユーザを取得することができるようになります。

データベースの権限を管理する
----------------------------

このチュートリアルの最後では、 データベースに権限のリストを格納したり、取得したりする方法を説明します。上記で説明したように、ユーザがロードされると、 ``getRoles()`` メソッドがそのユーザのセキュリティ権限の配列を返します。このセキュリティ権限はどこに格納していてもロードすることができます。それは、全てのユーザのためのハードコードでも、 Doctrine の配列プロパティの ``roles`` でも、 Doctrine の関連するからもです。それでは、その方法をこのセクションで見ていきましょう。

.. caution::

    標準的なセットアップでは、 ``getRoles()`` メソッドは必ず１つ以上の権限を返す必要があります。関連として、通常は ``ROLE_USER`` が返されます。権限を返すことに失敗すると、つまり、それはそのユーザは認証がされていないことになります。

この例では、 ``AcmeUserBundle:User`` エンティティクラスは、 ``AcmeUserBundle:Group`` エンティティクラスと多対多の関連しています。ユーザは、複数のグループに関連することができますし、グループも複数のユーザから成ることもできます。グループもまた１つの権限なので、 ``getRoles()`` メソッドで関連するグループのリストを返すようにします。
::

    // src/Acme/UserBundle/Entity/User.php

    namespace Acme\Bundle\UserBundle\Entity;

    use Doctrine\Common\Collections\ArrayCollection;

    // ...
    class User implements AdvancedUserInterface
    {
        /**
         * @ORM\ManyToMany(targetEntity="Group", inversedBy="users")
         *
         */
        private $groups;

        public function __construct()
        {
            $this->groups = new ArrayCollection();
        }

        // ...

        public function getRoles()
        {
            return $this->groups->toArray();
        }
    }

``AcmeUserBundle:Group`` エンティティクラスは、３つのテーブルフィールドを定義しています(``id``, ``name``, ``role``)。ユニークな ``role`` フィールドは、 Symfony アプリケーションのセキュアな部分への セキュリティレイヤーによって使用される権限の名前を含んでいます。最も重要なことは、 ``AcmeUserBundle:Group`` エンティティクラスが  :class:`Symfony\\Component\\Security\\Core\\Role\\RoleInterface` インタフェースを実装しており、 ``getRole()`` メソッドが強制となっていることです。
::

    namespace Acme\Bundle\UserBundle\Entity;

    use Symfony\Component\Security\Core\Role\RoleInterface;
    use Doctrine\Common\Collections\ArrayCollection;
    use Doctrine\ORM\Mapping as ORM;

    /**
     * @ORM\Table(name="acme_groups")
     * @ORM\Entity()
     */
    class Group implements RoleInterface
    {
        /**
         * @ORM\Column(name="id", type="integer")
         * @ORM\Id()
         * @ORM\GeneratedValue(strategy="AUTO")
         */
        private $id;

        /** @ORM\Column(name="name", type="string", length=30) */
        private $name;

        /** @ORM\Column(name="role", type="string", length=20, unique=true) */
        private $role;

        /** @ORM\ManyToMany(targetEntity="User", mappedBy="groups") */
        private $users;

        public function __construct()
        {
            $this->users = new ArrayCollection();
        }

        // ... getters and setters for each property

        /** @see RoleInterface */
        public function getRole()
        {
            return $this->role;
        }
    }

カスタムエンティティプロバイダからユーザを検索する際に、パフォーマンスを改良し、グループの遅延ローディングを避けるための最良の方法は、 ``UserRepository:loadUserByUsername()`` メソッドでグループリレーションをジョインすることです。こうすることで、１つのクエリーでユーザとそれに紐づいた権限やグループをまとめて取得します。
::

    // src/Acme/UserBundle/Entity/UserRepository.php

    namespace Acme\Bundle\UserBundle\Entity;

    // ...

    class UserRepository extends EntityRepository implements UserProviderInterface
    {
        public function loadUserByUsername($username)
        {
            $q = $this
                ->createQueryBuilder('u')
                ->select('u, g')
                ->leftJoin('u.groups', 'g')
                ->where('u.username = :username OR u.email = :email')
                ->setParameter('username', $username)
                ->setParameter('email', $username)
                ->getQuery()
            ;

            // ...
        }

        // ...
    }

email と username からユーザを取得する際に ``QueryBuilder::leftJoin()`` メソッドは、 ``AcmeUserBundle:User`` モデルクラスから、関連するグループをジョインし取得します。

.. 2012/01/04 ganchiku fddaa3f9210cb5626ec4bdc6ac0a2afe966d39c0

