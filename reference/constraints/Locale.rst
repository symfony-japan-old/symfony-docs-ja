Locale
======

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

値が有効なロケール文字列であることを検証します。

ロケールの値とは、2 文字の ISO639-1 *言語*\ コード (``fr``) か、言語コードにアンダースコア (``_``) をつなげ、その後に ISO3166 *国*\ コードをつなげたものです (French/France であれば ``fr_FR``)。

+----------------+------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`            |
+----------------+------------------------------------------------------------------------+
| オプション     | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Locale`            |
+----------------+------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\LocaleValidator`   |
+----------------+------------------------------------------------------------------------+

基本的な使い方
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\User:
            properties:
                locale:
                    - Locale:

    .. code-block:: php-annotations

        // src/Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class User
        {
            /**
             * @Assert\Locale
             */
             protected $locale;
        }

    .. code-block:: xml

        <!-- src/Acme/UserBundle/Resources/config/validation.xml -->
        <class name="Acme\UserBundle\Entity\User">
            <property name="locale">
                <constraint name="Locale" />
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
                $metadata->addPropertyConstraint('locale', new Assert\Locale());
            }
        }

オプション
----------

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This value is not a valid locale``

検証対象の文字列が有効なロケール文字列ではなかった場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto aad266861b7e44e188914b78b9de718b29e78dd7
