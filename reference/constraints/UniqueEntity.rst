UniqueEntity
============

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

Doctrine エンティティの特定のフィールド (1 つまたは複数) がマッピング先のテーブル内でユニークであることを検証します。たとえば、新規ユーザーがメールアドレスを使って登録する場合に、システムにすでに同じメールアドレスが登録されていないか調べる場合に使います。

+----------------+-------------------------------------------------------------------------------------+
| 適用先         | :ref:`クラス<validation-class-target>`                                              |
+----------------+-------------------------------------------------------------------------------------+
| オプション     | - `fields`_                                                                         |
|                | - `message`_                                                                        |
|                | - `em`_                                                                             |
|                | - `repositoryMethod`_                                                               |
|                | - `ignoreNull`_                                                                     |
+----------------+-------------------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Bridge\\Doctrine\\Validator\\Constraints\\UniqueEntity`            |
+----------------+-------------------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Bridge\\Doctrine\\Validator\\Constraints\\UniqueEntityValidator`   |
+----------------+-------------------------------------------------------------------------------------+

基本的な使い方
--------------

``AcmeUserBundle`` バンドルに ``email`` フィールドを持つ ``User`` エンティティがあるとします。\ ``UniqueEntity`` 制約を指定することで、ユーザーテーブル中で ``email`` フィールドの値がユニークであることを保証できます。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\Author:
            constraints:
                - Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity: email
            properties:
                email:
                    - Email: ~

    .. code-block:: php-annotations

        // Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;
        use Doctrine\ORM\Mapping as ORM;

        // DON'T forget this use statement!!!
        use Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity;

        /**
         * @ORM\Entity
         * @UniqueEntity("email")
         */
        class Author
        {
            /**
             * @var string $email
             *
             * @ORM\Column(name="email", type="string", length=255, unique=true)
             * @Assert\Email()
             */
            protected $email;
            
            // ...
        }

    .. code-block:: xml

        <class name="Acme\UserBundle\Entity\Author">
            <constraint name="Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity">
                <option name="fields">email</option>
                <option name="message">This email already exists.</option>
            </constraint>
            <property name="email">
                <constraint name="Email" />
            </property>
        </class>

    .. code-block:: php


        // Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        // DON'T forget this use statement!!!
        use Symfony\Bridge\Doctrine\Validator\Constraints\UniqueEntity;
        
        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addConstraint(new UniqueEntity(array(
                    'fields'  => 'email',
                    'message' => 'This email already exists.',
                )));

                $metadata->addPropertyConstraint(new Assert\Email());
            }
        }

オプション
----------

fields
~~~~~~

**型**: ``array`` | ``string`` [:ref:`default option<validation-default-option>`]

このオプションは必須で、エンティティーでユニークとするフィールド、またはフィールドのリストを指定します。たとえば、1 つの ``UniqueEntity`` 制約に ``email`` フィールドと ``name`` フィールドの両方を指定すると、これら 2 つのフィールドの値の組み合わせがユニークになります。2 つのユーザーが同じメールアドレスであっても、名前が同じでない限り登録できます。

2 つのフィールドに対して個々にユニークである必要がある場合 (``email`` のみでユニーク、\ ``username`` のみでユニーク) は、2 つの ``UniqueEntity`` を設定してください。

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is already used.``

検証に失敗した場合に、このメッセージが表示されます。

em
~~

**型**: ``string``

ユニークかどうかを決定するクエリーを実行するエンティティーマネージャー名を指定します。何も指定しない場合は適切なエンティティーマネージャーが選択されます。

repositoryMethod
~~~~~~~~~~~~~~~~

**型**: ``string`` **デフォルト**: ``findBy``

ユニークかどうかを決定するクエリーを実行するリポジトリーメソッド名を指定します。何も指定しない場合は ``findBy`` メソッドが使われます。指定したメソッドはカウント可能 (Countable) な結果を返す必要があります。

ignoreNull
~~~~~~~~~~

**型**: ``Boolean`` **デフォルト**: ``true``

このオプションを ``true`` に設定すると、対象のフィールドが ``null`` であるエンティティーが重複しても検証が失敗しません。このオプションを ``false`` に設定すると、\ ``null`` のエンティティーについても 1 つのみが許可され、2 つめのエンティティーでは検証が失敗します。

.. 2013/06/09 hidenorigoto bf1e0640d63c3dccd16b11a83e356e68dd97bd7f
