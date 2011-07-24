.. 2011/07/23 yanchi 57b7d4750ae7e2a2382ac0ad44fa82d4bfc7765e

MaxLength
=========

値の文字列の長さが、与えられた制限値以下であることを検証します。

+-----------------------+----------------------------------------------------------------+
| 検証                  | 文字列                                                         |
+-----------------------+----------------------------------------------------------------+
| オプション            | - ``limit``                                                    |
|                       | - ``message``                                                  |
|                       | - ``charset``                                                  |
+-----------------------+----------------------------------------------------------------+
| デフォルト オプション | ``limit``                                                      |
+-----------------------+----------------------------------------------------------------+
| クラス                | :class:`Symfony\\Component\\Validator\\Constraints\\MaxLength` |
+-----------------------+----------------------------------------------------------------+

オプション
----------

*   ``limit`` (**デフォルト**, 必須) [タイプ: 整数値]
    これは文字列の最大長です。 もし10に設定された場合、文字列は長さが
    10文字以内でなければなりません。

*   ``message`` [タイプ: 文字列, デフォルト: ``This value is too long. It should have {{ limit }} characters or less``]
    検証に失敗した場合のエラーメッセージ

基本的な使い方
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/validation.yml
        Acme\HelloBundle\Blog:
            properties:
                summary:
                    - MaxLength: 100
    
    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/validation.xml -->
        <class name="Acme\HelloBundle\Blog">
            <property name="summary">
                <constraint name="MaxLength">
                    <value>100</value>
                </constraint>
            </property>
        </class>

    .. code-block:: php-annotations

        // src/Acme/HelloBundle/Blog.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Blog
        {
            /**
             * @Assert\MaxLength(100)
             */
            protected $summary;
        }
