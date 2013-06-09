All
===

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/7

この制約を配列（または Traversable オブジェクト）に使うことで、制約のコレクションを配列の各要素に対して適用することができます。

+----------------+------------------------------------------------------------------------+
| 適用先         | :ref:`プロパティ又はメソッド<validation-property-target>`              |
+----------------+------------------------------------------------------------------------+
| オプション     | - `constraints`_                                                       |
+----------------+------------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\All`               |
+----------------+------------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\AllValidator`      |
+----------------+------------------------------------------------------------------------+

基本的な使い方
--------------

文字列を要素に持つ配列があり、各要素のバリデーションを行いたい場合、次のようにします。

.. configuration-block::

    .. code-block:: yaml

        # src/UserBundle/Resources/config/validation.yml
        Acme\UserBundle\Entity\User:
            properties:
                favoriteColors:
                    - All:
                        - NotBlank:  ~
                        - Length:
                            min: 5

    .. code-block:: php-annotations

        // src/Acme/UserBundle/Entity/User.php
        namespace Acme\UserBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;
  
        class User
        {
            /**
             * @Assert\All({
             *     @Assert\NotBlank
             *     @Assert\Length(min = "5"),
             * })
             */
             protected $favoriteColors = array();
        }

    .. code-block:: xml

        <!-- src/Acme/UserBundle/Resources/config/validation.xml -->
        <class name="Acme\UserBundle\Entity\User">
            <property name="favoriteColors">
                <constraint name="All">
                    <option name="constraints">
                        <constraint name="NotBlank" />
                        <constraint name="Length">
                            <option name="min">5</option>
                        </constraint>
                    </option>
                </constraint>
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
                $metadata->addPropertyConstraint('favoriteColors', new Assert\All(array(
                    'constraints' => array(
                        new Assert\NotBlank(),
                        new Assert\Length(array('min' => 5)),
                    ),
                )));
            }
        }

こうすると、\ ``favoriteColors`` 配列のすべての要素について、空でないこと、かつ 5 文字以上であることが検証されます。

オプション
----------

constraints
~~~~~~~~~~~

**型**: ``array`` [:ref:`default option<validation-default-option>`]

このオプションは必須で、配列の各要素に適用したいバリデーション制約の配列を指定します。

.. 2013/06/07 hidenorigoto 47007cf465f5ec69b181cf2a1dd5ae3d77b0a822
