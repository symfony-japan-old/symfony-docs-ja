.. 2011/07/24 yanchi 57b7d4750ae7e2a2382ac0ad44fa82d4bfc7765e

MinLength
=========

値の文字列の長さが、与えられた制限値以上であることを検証します。

+-----------------------+----------------------------------------------------------------+
| 検証                  | 文字列                                                         |
+-----------------------+----------------------------------------------------------------+
| オプション            | - ``limit``                                                    |
|                       | - ``message``                                                  |
|                       | - ``charset``                                                  |
+-----------------------+----------------------------------------------------------------+
| デフォルト オプション | ``limit``                                                      |
+-----------------------+----------------------------------------------------------------+
| クラス                | :class:`Symfony\\Component\\Validator\\Constraints\\MinLength` |
+-----------------------+----------------------------------------------------------------+

オプション
----------

*   ``limit`` (**デフォルト**, 必須) [タイプ: 整数値]
    これは文字列の最小長です。 もし3に設定された場合、文字列は長さが
    3文字以上でなければなりません。

*   ``message`` [タイプ: 文字列, デフォルト: ``This value is too short. It should have {{ limit }} characters or more``]
    検証に失敗した場合のエラーメッセージ

基本的な使い方
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/validation.yml
        Acme\HelloBundle\Author:
            properties:
                firstName:
                    - MinLength: 3
    
    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/validation.xml -->
        <class name="Acme\HelloBundle\Author">
            <property name="firstName">
                <constraint name="MinLength">
                    <value>3</value>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HelloBundle/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\MinLength(3)
             */
            protected $firstName;
        }
