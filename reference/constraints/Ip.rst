Ip
==

.. note::

    * 対象バージョン：2.3
    * 翻訳更新日：2013/6/9

値が有効な IP アドレスであることを検証します。デフォルトでは、IPv4 のアドレスとして検証されますが、オプションにより IPv6 のアドレスとして、またはそれらの組み合わせとして検証することもできます。

+----------------+---------------------------------------------------------------------+
| 適用先         | :ref:`プロパティまたはメソッド<validation-property-target>`         |
+----------------+---------------------------------------------------------------------+
| オプション     | - `version`_                                                        |
|                | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| クラス         | :class:`Symfony\\Component\\Validator\\Constraints\\Ip`             |
+----------------+---------------------------------------------------------------------+
| バリデーター   | :class:`Symfony\\Component\\Validator\\Constraints\\IpValidator`    |
+----------------+---------------------------------------------------------------------+

基本的な使い方
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                ipAddress:
                    - Ip:

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Ip
             */
             protected $ipAddress;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="ipAddress">
                <constraint name="Ip" />
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints as Assert;
  
        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('ipAddress', new Assert\Ip());
            }
        }

オプション
----------

version
~~~~~~~

**型**: ``string`` **デフォルト**: ``4``

IP アドレスの検証方法を、以下のどれか 1 つで指定します。

**すべての範囲**

* ``4`` - IPv4 アドレスとして検証します。
* ``6`` - IPv6 アドレスとして検証します。
* ``all`` - すべての IP フォーマットとして検証します。

**プライベートアドレスを含まない範囲**

* ``4_no_priv`` - プライベート IP アドレスの範囲を含まない IPv4 アドレスとして検証します。
* ``6_no_priv`` - プライベート IP アドレスの範囲を含まない IPv6 アドレスとして検証します。
* ``all_no_priv`` - プライベート IP アドレスの範囲を含まないすべてのフォーマットの IP アドレスとして検証します。

**予約済みのアドレスを含まない範囲**

* ``4_no_res`` - 予約済み IP アドレスの範囲を含まない IPv4 アドレスとして検証します。
* ``6_no_res`` - 予約済み IP アドレスの範囲を含まない IPv6 アドレスとして検証します。
* ``all_no_res`` - 予約済み IP アドレスの範囲を含まないすべてのフォーマットの IP アドレスとして検証します。

**パブリックアドレスの範囲**

* ``4_public`` - プライベートと予約済みを除くパブリックな IP アドレスの範囲のみの IPv4 アドレスとして検証します。
* ``6_public`` - プライベートと予約済みを除くパブリックな IP アドレスの範囲のみの IPv6 アドレスとして検証します。
* ``all_public`` - プライベートと予約済みを除くパブリックな IP アドレス範囲のみのすべてのフォーマットの IP アドレスとして検証します。

message
~~~~~~~

**型**: ``string`` **デフォルト**: ``This is not a valid IP address``

値が有効な IP アドレスではかなった場合に、このメッセージが表示されます。

.. 2013/06/09 hidenorigoto aad266861b7e44e188914b78b9de718b29e78dd7
