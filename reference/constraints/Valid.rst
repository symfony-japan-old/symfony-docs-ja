Valid
=====

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

この制約を使うと、あるオブジェクトのプロパティとしてオブジェクトが埋め込まれている場合に、埋め込まれたオブジェクトに対して指定された検証も行われます。オブジェクトと、関連付けられたサブオブジェクトのすべてを検証できます。

+----------------+---------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`         |
+----------------+---------------------------------------------------------------------+
| オプション     | - `traverse`_                                                       |
+----------------+---------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Type`           |
+----------------+---------------------------------------------------------------------+

基本的な使い方
--------------

次の例では ``Author`` クラスと ``Address`` クラスを作成し、それぞれのプロパティに制約を設定します。さらに、\ ``Author`` の ``$address`` プロパティには ``Address`` インスタンスが保存されています。

.. code-block:: php

    // src/Acme/HelloBundle/Entity/Address.php
    namespace Amce\HelloBundle\Entity;

    class Address
    {
        protected $street;
        protected $zipCode;
    }

.. code-block:: php

    // src/Acme/HelloBundle/Entity/Author.php
    namespace Amce\HelloBundle\Entity;

    class Author
    {
        protected $firstName;
        protected $lastName;
        protected $address;
    }

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/validation.yml
        Acme\HelloBundle\Entity\Address:
            properties:
                street:
                    - NotBlank: ~
                zipCode:
                    - NotBlank: ~
                    - Length:
                        max: 5

        Acme\HelloBundle\Entity\Author:
            properties:
                firstName:
                    - NotBlank: ~
                    - Length:
                        min: 4
                lastName:
                    - NotBlank: ~

    .. code-block:: php-annotations

        // src/Acme/HelloBundle/Entity/Address.php
        namespace Acme\HelloBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Address
        {
            /**
             * @Assert\NotBlank()
             */
            protected $street;

            /**
             * @Assert\NotBlank
             * @Assert\Length(max = "5")
             */
            protected $zipCode;
        }

        // src/Acme/HelloBundle/Entity/Author.php
        namespace Acme\HelloBundle\Entity;

        class Author
        {
            /**
             * @Assert\NotBlank
             * @Assert\Length(min = "4")
             */
            protected $firstName;

            /**
             * @Assert\NotBlank
             */
            protected $lastName;

            protected $address;
        }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/validation.xml -->
        <class name="Acme\HelloBundle\Entity\Address">
            <property name="street">
                <constraint name="NotBlank" />
            </property>
            <property name="zipCode">
                <constraint name="NotBlank" />
                <constraint name="Length">
                    <option name="max">5</option>
                </constraint>
            </property>
        </class>

        <class name="Acme\HelloBundle\Entity\Author">
            <property name="firstName">
                <constraint name="NotBlank" />
                <constraint name="Length">
                    <option name="min">4</option>
                </constraint>
            </property>
            <property name="lastName">
                <constraint name="NotBlank" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/HelloBundle/Entity/Address.php
        namespace Acme\HelloBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Address
        {
            protected $street;
            protected $zipCode;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('street', new Assert\NotBlank());
                $metadata->addPropertyConstraint('zipCode', new Assert\NotBlank());
                $metadata->addPropertyConstraint(
                    'zipCode',
                    new Assert\Length(array("max" => 5)));
            }
        }

        // src/Acme/HelloBundle/Entity/Author.php
        namespace Acme\HelloBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            protected $firstName;
            protected $lastName;
            protected $address;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new Assert\NotBlank());
                $metadata->addPropertyConstraint('firstName', new Assert\Length(array("min" => 4)));
                $metadata->addPropertyConstraint('lastName', new Assert\NotBlank());
            }
        }

このマッピングだけでは、無効なアドレスを持った著者でも検証をパスしてしまいます。これを回避するには、\ ``$address`` プロパティに ``Valid`` 制約を追加します。

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/validation.yml
        Acme\HelloBundle\Author:
            properties:
                address:
                    - Valid: ~

    .. code-block:: php-annotations

        // src/Acme/HelloBundle/Entity/Author.php
        namespace Acme\HelloBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Valid
             */
            protected $address;
        }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/validation.xml -->
        <class name="Acme\HelloBundle\Entity\Author">
            <property name="address">
                <constraint name="Valid" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/HelloBundle/Entity/Author.php
        namespace Acme\HelloBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            protected $address;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('address', new Assert\Valid());
            }
        }

これで、無効なアドレスを持つ著者を検証すると、次のように ``Address`` のフィールドで検証に失敗したことが表示されます。

    Acme\HelloBundle\Author.address.zipCode:
    This value is too long. It should have 5 characters or less

オプション
----------

traverse
~~~~~~~~

**型**: ``boolean`` **デフォルト**: ``true``

このオプションを ``true`` に設定すると、オブジェクトの配列が格納されたプロパティに対して制約を適用した場合に、配列の各要素のオブジェクトに対しても検証が行われます。

.. 2013/06/09 hidenorigoto 18173a5ba5af5ffb3f68a9c1a561d5907e9d2eb2
