Country
=======

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/7

値が有効な 2 文字の国コードであるか検証します。

+----------------+------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`            |
+----------------+------------------------------------------------------------------------+
| オプション     | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Country`           |
+----------------+------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\CountryValidator`  |
+----------------+------------------------------------------------------------------------+

基本的な使い方
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\User:
            properties:
                country:
                    - Country:

    .. code-block:: php-annotations

        // src/Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;

        use Symfony\Component\Validator\Constraints as Assert;

        class User
        {
            /**
             * @Assert\Country
             */
             protected $country;
        }

    .. code-block:: xml

        <!-- src/Acme/UserBundle/Resources/config/validation.xml -->
        <class name="Acme\UserBundle\Entity\User">
            <property name="country">
                <constraint name="Country" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class User
        {
            public static function loadValidationMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('country', new Assert\Country());
            }
        }

オプション
----------

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is not a valid country``

文字列が有効な国コードではない場合に、このメッセージが表示されます。

.. 2013/06/07 hidenorigoto 9cabf0df7098a47e28515000b90f9c7446c17d6e
