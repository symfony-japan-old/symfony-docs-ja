Language
========

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

値が有効な言語コードであることを検証します。

+----------------+------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`            |
+----------------+------------------------------------------------------------------------+
| オプション     | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Language`          |
+----------------+------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\LanguageValidator` |
+----------------+------------------------------------------------------------------------+

基本的な使い方
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\User:
            properties:
                preferredLanguage:
                    - Language:

    .. code-block:: php-annotations

        // src/Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;
  
        class User
        {
            /**
             * @Assert\Language
             */
             protected $preferredLanguage;
        }

    .. code-block:: xml

        <!-- src/Acme/UserBundle/Resources/config/validation.xml -->
        <class name="Acme\UserBundle\Entity\User">
            <property name="preferredLanguage">
                <constraint name="Language" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;
        
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;

        class User
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('preferredLanguage', new Assert\Language());
            }
        }

オプション
----------

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is not a valid language``

検証対象の文字列が有効な言語コードではなかった場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto aad266861b7e44e188914b78b9de718b29e78dd7
